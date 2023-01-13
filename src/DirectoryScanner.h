#pragma once

#include "File.h"

#include <boost/filesystem.hpp>

#include <vector>
#include <string>
#include <memory>

namespace Homework {

class DirectoryScanner
{
public:
    static const std::size_t DEFAULT_SCAN_LEVEL;
    static const FileSize DEFAULT_MIN_FILE_SIZE_BYTE;
    static const FileSize DEFAULT_BLOCK_SIZE_BYTE;

    std::vector<std::unique_ptr<File>> getFilesFromDirectories() const;

    void setDirectoriesToScan(const std::vector<std::string>& directoriesToScan);
    void setDirectoriesToExclude(const std::vector<std::string>& directoriesToExclude);
    void setFileMasks(const std::vector<std::string>& fileMasks);
    void setScanLevel(std::size_t scanLevel);
    void setMinFileSize(FileSize minFileSize);
    void setBlockSize(FileSize blockSize);
    void setHasher(const Hasher& hasher);
private:
    std::vector<std::string> directoriesToScan;
    std::vector<boost::filesystem::path> directoriesToExclude;
    std::vector<std::string> fileMasks;
    std::size_t scanLevel = DEFAULT_SCAN_LEVEL;
    FileSize minFileSize = DEFAULT_MIN_FILE_SIZE_BYTE;
    FileSize blockSize = DEFAULT_BLOCK_SIZE_BYTE;
    Hasher hasher;

    void collectFiles(const boost::filesystem::path& rootDirectory, std::size_t currentScanLevel, std::vector<std::unique_ptr<File>>& target) const;
    bool isDirectoryExcluded(const boost::filesystem::path& directory) const;
    bool matchesAnyMask(const std::string& filename) const;
};

};
