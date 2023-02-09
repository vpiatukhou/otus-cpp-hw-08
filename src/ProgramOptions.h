#pragma once

#include "File.h"
#include "HashAlgorithmType.h"

#include <ostream>

namespace Homework {

class ProgramOptions {
public:
    bool parse(int argc, char* argv[], std::ostream& out);

    std::vector<std::string> getDirectoriesToScan() const;
    std::vector<std::string> getDirectoriesToExclude() const;
    std::vector<std::string> getFileMasks() const;
    std::size_t getScanLevel() const;
    FileSize getMinFileSize() const;
    FileSize getBlockSize() const;
    HashAlgorithmType getHashAlgorithm() const;
private:
    static const std::string HELP_PARAM;
    static const std::size_t DEFAULT_SCAN_LEVEL;
    static const FileSize DEFAULT_MIN_FILE_SIZE_BYTE;
    static const std::size_t DEFAULT_BLOCK_SIZE_BYTE;

    static const std::string HASH_ALGORITHM_CRC32;
    static const std::string HASH_ALGORITHM_MD5;

    std::vector<std::string> directoriesToScan;
    std::vector<std::string> directoriesToExclude;
    std::vector<std::string> fileMasks;
    std::size_t scanLevel = 0;
    FileSize minFileSize = 0;
    FileSize blockSize = 0;
    HashAlgorithmType hashAlgorithm;
};

};
