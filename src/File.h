#pragma once

#include "Hasher.h"

#include <cstddef>
#include <string>

namespace Homework {

using FileSize = std::uintmax_t; //this type is returned by boost::filesystem::file_size()

class File {
public:
    virtual ~File() {
    }

    /**
     * Reads a next block.
     * 
     * @param blockSize - a size of the block
     * @param hasher    - generates a hash of the block
     * @return TRUE if the block has been read. FALSE if there is nothing to read.
     */
    virtual bool readNextBlock(FileSize blockSize, const Hasher& hasher) = 0;

    virtual std::string const& getFilepath() const = 0;
    virtual FileSize getFileSize() const = 0;

    /**
     * @return the hash of the last block which was read by readNextBlock().
     */
    virtual Hash const& getBlockHash() const = 0;
};

/**
 * Compares hashes which are returned by getBlockHash() of the given files.
 * 
 * @return TRUE if the hashes are equal
 */
bool operator==(const File& a, const File& b);

/**
 * Compares hashes which are returned by getBlockHash() of the given files.
 * 
 * @return TRUE if the hash of the first file is less than the hash of the second one
 */
bool operator<(const File& a, const File& b);

};
