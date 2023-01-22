#pragma once

#include "File.h"

#include "ostream"

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
    std::string getHashAlgorithm() const;
private:
    std::vector<std::string> directoriesToScan;
    std::vector<std::string> directoriesToExclude;
    std::vector<std::string> fileMasks;
    std::size_t scanLevel = 0;
    FileSize minFileSize = 0;
    FileSize blockSize = 0;
    std::string hashAlgorithm;
};

};
