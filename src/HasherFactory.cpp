#include "HasherFactory.h"

#include <boost/algorithm/string.hpp>
#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>

#include <stdexcept>

namespace Homework {

const std::size_t MD5_DIGEST_SIZE = 4;

using BytesToHashRef = const std::vector<char>&;

Hash getCrcHash(BytesToHashRef bytesToHash) {
    boost::crc_32_type computer;
    computer.process_bytes(bytesToHash.data(), bytesToHash.size());
    return { computer.checksum() };
}

Hash getMd5Hash(BytesToHashRef bytesToHash) {
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

Hasher getHasher(const std::string& algorithm) {
    Hasher hasher;
    if (boost::iequals(CRC32, algorithm)) {
        hasher = getCrcHash;
    } else if (boost::iequals(MD5, algorithm)) {
        hasher = getMd5Hash;
    } else {
        throw std::invalid_argument("Unsupported algorithm: " + algorithm);
    }
    return hasher;
}

};
