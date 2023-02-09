#pragma once

#include "File.h"

#include <cstddef>
#include <string>

namespace Homework {

    class FileImpl : public File {
    public:
        FileImpl(const std::string& filepath, FileSize fileSize);

        bool readNextBlock(FileSize blockSize, const Hasher& hasher) override;

        std::string const& getFilepath() const override;
        FileSize getFileSize() const override;
        Hash const& getBlockHash() const override;
    private:
        std::string filepath;
        std::size_t offset = 0;
        FileSize fileSize = 0;
        Hash blockHash;
    };

};
