#include "HasherFactory.h"

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>

#include <stdexcept>

namespace Homework {

const std::size_t MD5_DIGEST_SIZE = 4;

using BytesToHashRef = const std::vector<char>&;

Hash generateCrc32Hash(BytesToHashRef bytesToHash) {
    boost::crc_32_type computer;
    computer.process_bytes(bytesToHash.data(), bytesToHash.size());
    return { computer.checksum() };
}

Hash generateMd5Hash(BytesToHashRef bytesToHash) {
    boost::uuids::detail::md5 computer;
    boost::uuids::detail::md5::digest_type digest;
    computer.process_bytes(bytesToHash.data(), bytesToHash.size());
    computer.get_digest(digest);

    Hash hash;
    for (std::size_t i = 0; i < MD5_DIGEST_SIZE; ++i) {
        hash.push_back(digest[i]);
    }
    return hash;
}

Hasher createHasher(HashAlgorithmType algorithm) {
    Hasher hasher;
    switch (algorithm) {
    case HashAlgorithmType::CRC32:
        hasher = generateCrc32Hash;
        break;
    case HashAlgorithmType::MD5:
        hasher = generateMd5Hash;
        break;
    default:
        throw std::invalid_argument("Unsupported hash algorithm.");
    }
    return hasher;
}

};
