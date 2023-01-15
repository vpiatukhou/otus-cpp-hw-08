#pragma once

#include <regex>
#include <string>
#include <vector>

namespace Homework {

class StringMatcher {

public:
    StringMatcher(const std::vector<std::string>& masks);

    virtual bool matches(const std::string& str) const;
private:
    std::vector<std::regex> masks;

    void replaceAnCharWildcard(std::string& mask) const;
};

}
