#pragma once

#include "File.h"

#include <string>

namespace Homework {

bool matchesWithMask(const std::string& str, const std::string& mask);

FileSize fileSizeToBytes(const std::string& sizeAsStr);

};
