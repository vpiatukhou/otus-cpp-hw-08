#include "File.h"

namespace Homework {

bool operator==(const File& a, const File& b) {
    return a.getBlockHash() == b.getBlockHash();
}

bool operator<(const File& a, const File& b) {
    return a.getBlockHash() < b.getBlockHash();
}

}
