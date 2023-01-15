#include "FileImpl.h"
#include "DirectoryScanner.h"

#include <cassert>
#include <limits>
#include <stack>

namespace Homework {

namespace fs = boost::filesystem;

const std::size_t DirectoryScanner::DEFAULT_SCAN_LEVEL = 0;
const FileSize DirectoryScanner::DEFAULT_MIN_FILE_SIZE_BYTE = 2;
const std::size_t DirectoryScanner::DEFAULT_BLOCK_SIZE_BYTE = 8;

std::vector<std::unique_ptr<File>> DirectoryScanner::getFilesFromDirectories() const {
    assert(filenameMatcher != nullptr);

    std::vector<std::unique_ptr<File>> target;
    for (auto& directory : directoriesToScan) {
        if (fs::is_directory(directory)) { //skip an invalid path
            collectFiles(directory, 0, target);
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
        } else if (fs::is_regular_file(path) && filenameMatcher->matches(path.filename().string())) {
            auto fileSize = fs::file_size(path);
            if (fileSize >= minFileSize) {
                target.push_back(std::make_unique<FileImpl>(path.string(), blockSize, fileSize, hasher));
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
