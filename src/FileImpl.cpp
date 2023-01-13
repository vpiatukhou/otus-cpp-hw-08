#include "FileImpl.h"

#include <cassert>
#include <fstream>
#include <vector>

namespace Homework {

    FileImpl::FileImpl(const std::string& filepath_, FileSize blockSize_, FileSize fileSize_, const Hasher& checksumGenerator_)
        : filepath(filepath_), blockSize(blockSize_), hasher(checksumGenerator_), fileSize(fileSize_) {
    }

    bool FileImpl::readNextBlock() {
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
