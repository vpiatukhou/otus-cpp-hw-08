#include "FileImpl.h"

#include <cassert>
#include <fstream>
#include <vector>

namespace Homework {

FileImpl::FileImpl(const std::string& filepath_, FileSize fileSize_) : filepath(filepath_), fileSize(fileSize_) {
}

bool FileImpl::readNextBlock(FileSize blockSize, const Hasher& hasher) {
    if (offset >= fileSize) {
        return false;
    }

    std::vector<char> block;
    block.resize(blockSize, 0); //initialize block with zeros

    std::ifstream istream(filepath);
    istream.seekg(offset);
    istream.read(block.data(), blockSize);
    istream.close();

    blockHash = hasher(block);

    offset += blockSize;
    return offset < fileSize;
}

std::string const& FileImpl::getFilepath() const {
    return filepath;
}

FileSize FileImpl::getFileSize() const {
    return fileSize;
}

Hash const& FileImpl::getBlockHash() const {
    return blockHash;
}

};
