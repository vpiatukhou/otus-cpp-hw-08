#pragma once

#include "File.h"
#include "StringMatcher.h"

#include <boost/filesystem.hpp>

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

namespace Homework {

class DirectoryScanner
{
public:
    std::vector<std::unique_ptr<File>> getFilesFromDirectories() const;

    void setDirectoriesToScan(const std::vector<std::string>& directoriesToScan);
    void setDirectoriesToExclude(const std::vector<std::string>& directoriesToExclude);
    void setScanLevel(std::size_t scanLevel);
    void setMinFileSize(FileSize minFileSize);
    void setFilenameMatcher(const std::shared_ptr<StringMatcher>& filenameMatcher);
private:
    std::vector<std::string> directoriesToScan;
    std::vector<boost::filesystem::path> directoriesToExclude;
    std::size_t scanLevel = 0;
    FileSize minFileSize = 0;
    std::shared_ptr<StringMatcher> filenameMatcher;

    void collectFiles(const boost::filesystem::path& rootDirectory, std::vector<std::unique_ptr<File>>& target) const;
    bool isDirectoryExcluded(const boost::filesystem::path& directory) const;
};

};
