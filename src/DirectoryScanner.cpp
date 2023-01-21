#include "FileImpl.h"
#include "DirectoryScanner.h"

#include <cassert>
#include <limits>
#include <stack>

namespace Homework {

namespace fs = boost::filesystem;

const std::size_t DirectoryScanner::DEFAULT_SCAN_LEVEL = 0;
const FileSize DirectoryScanner::DEFAULT_MIN_FILE_SIZE_BYTE = 2;
const std::size_t DirectoryScanner::DEFAULT_BLOCK_SIZE_BYTE = 1024;

std::vector<std::unique_ptr<File>> DirectoryScanner::getFilesFromDirectories() const {
    assert(filenameMatcher != nullptr);

    std::vector<std::unique_ptr<File>> target;
    if (directoriesToScan.empty()) {
        //if no directories are specified, scan the current directory
        collectFiles(fs::current_path(), 0, target);
    } else {
        for (auto& directory : directoriesToScan) {
            if (fs::is_directory(directory)) { //skip an invalid path
                collectFiles(directory, 0, target);
            }
        }
    }
    return target;
}

void DirectoryScanner::collectFiles(const boost::filesystem::path& rootDirectory, std::size_t currentScanLevel, std::vector<std::unique_ptr<File>>& target) const {
    if (isDirectoryExcluded(rootDirectory)) {
        return;
    }

    for (auto& entry : fs::directory_iterator(rootDirectory)) {
        auto& path = entry.path();

        if (fs::is_directory(path) && currentScanLevel < scanLevel) {
            collectFiles(path, currentScanLevel + 1, target);
        } else if (fs::is_regular_file(path) && filenameMatcher->matches(path.filename().string()) 
            && !isFileCollected(target, path)) { //a file can be visited twice if directoriesToScan contains a directory AND its subdirectories

            auto fileSize = fs::file_size(path);
            if (fileSize >= minFileSize) {
                auto canonicalPath = fs::canonical(path).string();
                target.push_back(std::make_unique<FileImpl>(canonicalPath, blockSize, fileSize, hasher));
            }
        }
    }
}

bool DirectoryScanner::isDirectoryExcluded(const boost::filesystem::path& directory) const {
    for (auto& directoryToCompare : directoriesToExclude) {
        if (fs::equivalent(directoryToCompare, directory)) {
            return true;
        }
    }
    return false;
}

bool DirectoryScanner::isFileCollected(const std::vector<std::unique_ptr<File>>& target, const boost::filesystem::path& filepath) const {
    for (auto& file : target) {
        if (fs::equivalent(file->getFilepath(), filepath)) {
            return true;
        }
    }
    return false;
}

void DirectoryScanner::setDirectoriesToScan(const std::vector<std::string>& directoriesToScan) {
    this->directoriesToScan = directoriesToScan;
}

void DirectoryScanner::setDirectoriesToExclude(const std::vector<std::string>& directoriesToExclude) {
    std::vector<fs::path> paths;
    for (auto& directory : directoriesToExclude) {
        paths.push_back(directory);
    }
    this->directoriesToExclude = paths;
}

void DirectoryScanner::setScanLevel(std::size_t scanLevel) {
    this->scanLevel = scanLevel;
}

void DirectoryScanner::setMinFileSize(FileSize minFileSize) {
    this->minFileSize = minFileSize;
}

void DirectoryScanner::setBlockSize(FileSize blockSize) {
    this->blockSize = blockSize;
}

void DirectoryScanner::setFilenameMatcher(const std::shared_ptr<StringMatcher>& filenameMatcher) {
    this->filenameMatcher = filenameMatcher;
}

void DirectoryScanner::setHasher(const Hasher& hasher) {
    this->hasher = hasher;
}

};
