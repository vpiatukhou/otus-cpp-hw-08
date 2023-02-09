#pragma once

#include <cstdint>
#include <functional>
#include <vector>

/**
 * A hash. The different algorithms may produce hashes of different sizes, this is why std::vector was chosen.
 */
using Hash = std::vector<std::uint32_t>;

/**
 * A functional object. Returns a hash of the given sequence of chars.
 */
using Hasher = std::function<Hash(const std::vector<char>&)>;
