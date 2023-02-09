#pragma once

#include "File.h"

#include <string>

namespace Homework {

FileSize fileSizeToBytes(const std::string& sizeAsStr);

std::size_t stringToSize(const std::string& value);

};
