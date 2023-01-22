#pragma once

#include "HashAlgorithmType.h"
#include "Hasher.h"

#include <string>

namespace Homework {

Hasher createHasher(HashAlgorithmType algorithm);

};
