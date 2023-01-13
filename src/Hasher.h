#pragma once

#include <cstdint>
#include <functional>
#include <vector>

using Hash = std::vector<std::uint32_t>;
using Hasher = std::function<Hash(const std::vector<char>&)>;
