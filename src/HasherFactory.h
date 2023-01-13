#pragma once

#include "Hasher.h"

#include <string>

namespace Homework {

const std::string CRC32 = "crc32";
const std::string MD5 = "md5";

Hasher getHasher(const std::string& algorithm);

};