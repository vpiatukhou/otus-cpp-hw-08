#include "DirectoryScanner.h"
#include "FileImpl.h"

#include <boost/algorithm/string/predicate.hpp>

#include <algorithm>
#include <cassert>

namespace Homework {

namespace fs = boost::filesystem;

std::vector<std::unique_ptr<File>> DirectoryScanner::getFilesFromDirectories() const {
    assert(filenameMatcher != nullptr);

    std::vector<std::unique_ptr<File>> target;
    if (directoriesToScan.empty()) {
        //if no directories are specified, scan the current directory
        collectFiles(fs::current_path(), target);
    } else {
        for (auto& directory : directoriesToScan) {
            if (fs::is_directory(directory)) { //skip an invalid path
                collectFiles(directory, target);
            }
        }
    }

    //remove files with equivalent paths
    std::sort(target.begin(), target.end(), [](const auto& a, const auto& b) {
        return boost::algorithm::ilexicographical_compare(a->getFilepath(), b->getFilepath());
    });
    auto last = std::unique(target.begin(), target.end(), [](const auto& a, const auto& b) {
        return fs::equivalent(a->getFilepath(), b->getFilepath());
    });
    target.erase(last, target.end());

    return target;
}

void DirectoryScanner::collectFiles(const boost::filesystem::path& rootDirectory, std::vector<std::unique_ptr<File>>& target) const {
    fs::recursive_directory_iterator dirIt(rootDirectory);
    fs::recursive_directory_iterator dirEndIt;
    for (; dirIt != dirEndIt; ++dirIt) {
        auto& path = dirIt->path();
        if (dirIt.level() > scanLevel || isDirectoryExcluded(path)) {
            dirIt.no_push();
        } else if (fs::is_regular_file(path) && filenameMatcher->matches(path.filename().string())) {
            auto fileSize = fs::file_size(path);
            if (fileSize >= minFileSize) {
                auto canonicalPath = fs::canonical(path).string();
                //The directory may be visited more than once if directoriesToScan contains a directory AND its subdirectory.
                //In that case the same file may be added to 'target' multiple times.
                //We don't test if the file is already added in order to avoid a linear search in 'target'.
                //We cannot use std::unordered_set instead of std::vector because fs::path must be compared using fs::equivalent.
                //So we just add the same file twice and remove the duplicate later.
                target.push_back(std::make_unique<FileImpl>(canonicalPath, fileSize));
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

void DirectoryScanner::setFilenameMatcher(const std::shared_ptr<StringMatcher>& filenameMatcher) {
    this->filenameMatcher = filenameMatcher;
}

};
