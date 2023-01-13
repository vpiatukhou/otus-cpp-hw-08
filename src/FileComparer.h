#pragma once

#include "File.h"

#include <string>
#include <list>
#include <vector>
#include <memory>

namespace Homework {

std::list<std::list<std::string>> findDuplicateFiles(std::vector<std::unique_ptr<File>>& target);

};
