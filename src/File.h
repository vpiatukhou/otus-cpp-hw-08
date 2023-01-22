#pragma once

#include "Hasher.h"

#include <cstddef>
#include <string>

namespace Homework {

using FileSize = std::uintmax_t; //the same type is returned by boost::filesystem::file_size()

class File
{
public:
    virtual ~File() {
    }

    virtual bool readNextBlock(FileSize blockSize, const Hasher& hasher) = 0;

    virtual std::string const& getFilepath() const = 0;
    virtual FileSize getFileSize() const = 0;
    virtual Hash const& getBlockHash() const = 0;
};

bool operator==(const File& a, const File& b);
bool operator<(const File& a, const File& b);

};
