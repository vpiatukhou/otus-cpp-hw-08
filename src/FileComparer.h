#pragma once

#include "File.h"

#include <list>
#include <string>
#include <vector>
#include <memory>

namespace Homework {

class FileComparer {
public:
    FileComparer(FileSize blockSize_, const Hasher& hasher_);

    std::list<std::list<std::string>> findDuplicateFiles(std::vector<std::unique_ptr<File>>& target) const;
private:
    FileSize blockSize;
    Hasher hasher;

    void findAndAppendDuplicates(std::list<std::unique_ptr<File>>& files, std::list<std::list<std::string>>& target) const;
    bool readNextBlock(std::list<std::unique_ptr<File>>& files) const;
    void filesToPaths(const std::list<std::list<std::unique_ptr<File>>>& files, std::list<std::list<std::string>>& target) const;
};

};
