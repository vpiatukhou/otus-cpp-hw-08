#pragma once

#include "File.h"

#include <list>
#include <string>
#include <vector>
#include <memory>

namespace Homework {

class FileComparer {
public:
    //the group contains paths of the files with the same content
    using GroupOfPaths = std::list<std::string>;

    FileComparer(FileSize blockSize_, const Hasher& hasher_);

    std::list<GroupOfPaths> findDuplicateFiles(std::vector<std::unique_ptr<File>>& target) const;
private:
    //the group contains files with the same content
    using GroupOfFiles = std::list<std::unique_ptr<File>>;

    FileSize blockSize;
    Hasher hasher;

    void findAndAppendDuplicates(GroupOfFiles& files, std::list<GroupOfPaths>& target) const;
    bool readNextBlock(GroupOfFiles& files) const;
    void filesToPaths(const std::list<GroupOfFiles>& files, std::list<GroupOfPaths>& target) const;
};

};
