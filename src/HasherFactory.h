#pragma once

#include "HashAlgorithmType.h"
#include "Hasher.h"

namespace Homework {

/**
 * Creates a hasher which implements the given algorithm.
 */
Hasher createHasher(HashAlgorithmType algorithm);

};
