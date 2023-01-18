#include "StringMatcher.h"

namespace Homework {

// any of the following characters: \.[]{}()+^$|
// '/' is ignored if it is followed by '?'
const std::regex SPECIAL_SYMBOL_REGEX("(\\\\[^?])|\\.|\\[|\\]|\\{|\\}|\\(|\\)|\\+|\\^|\\$|\\|");
const std::regex ANY_CHAR_SEQ_WILDCARD("[*]");
const std::regex ANY_CHAR_WILDCARD("([^\\\\]|^)([?])");
const std::size_t ANY_CHAR_WILDCARD_GROUP_INDEX = 2;
const std::string ESCAPED_SYMBOL = "\\$&";
const std::string ANY_CHAR_SEQ_REGEXP = ".*";
const char ANY_CHAR_REGEXP = '.';

StringMatcher::StringMatcher(const std::vector<std::string>& masks) {
    std::vector<std::regex> regExMasks;
    regExMasks.reserve(masks.size());

    for (auto& mask : masks) {
        //escape special characters
        auto adjustedMask = std::regex_replace(mask, SPECIAL_SYMBOL_REGEX, ESCAPED_SYMBOL);

        //replace wildcards with regex expression
        adjustedMask = std::regex_replace(adjustedMask, ANY_CHAR_SEQ_WILDCARD, ANY_CHAR_SEQ_REGEXP);
        replaceAnCharWildcard(adjustedMask);

        std::regex maskRegEx(adjustedMask, std::regex_constants::ECMAScript | std::regex_constants::icase);
        regExMasks.push_back(maskRegEx);
    }

    this->masks = regExMasks;
}

void StringMatcher::replaceAnCharWildcard(std::string& mask) const {
    auto begin = std::sregex_iterator(mask.cbegin(), mask.cend(), ANY_CHAR_WILDCARD);
    auto end = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        auto wildcardPosition = (*it).position(ANY_CHAR_WILDCARD_GROUP_INDEX);
        mask[wildcardPosition] = ANY_CHAR_REGEXP;
    }
}

bool StringMatcher::matches(const std::string& str) const {
    if (masks.empty()) {
        return true;
    }
    for (auto& mask : masks) {
        std::smatch match;
        if (std::regex_match(str, match, mask)) {
            return true;
        }
    }
    return false;
}

}
