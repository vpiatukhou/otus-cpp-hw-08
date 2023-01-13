#include "Util.h"

#include <boost/lexical_cast.hpp>

#include <cctype>
#include <regex>

namespace Homework {

const std::regex SPECIAL_CHAR_REGEX("\\.|\\^|\\$|\\+|\\-\\?|\\||\\\\\\[|\\]|\\(|\\)|\\{|\\}");
const std::regex WILDCARD_REGEX("[*]");

const char SIZE_SUFFIX_KiB = 'k';
const char SIZE_SUFFIX_MiB = 'm';
const char SIZE_SUFFIX_GiB = 'g';

const FileSize BYTES_TO_KiB = 1024;
const FileSize BYTES_TO_MiB = 1024 * 1024;
const FileSize BYTES_TO_GiB = 1024 * 1024 * 1024;

bool matchesWithMask(const std::string& str, const std::string& mask) {
    //escape special characters
    std::string adjustedMask = std::regex_replace(mask, SPECIAL_CHAR_REGEX, "\\$&");
    //replace '*' with '.*'
    adjustedMask = std::regex_replace(adjustedMask, WILDCARD_REGEX, ".*");
    std::regex maskRegEx(adjustedMask, std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::smatch match;
    return std::regex_match(str, match, maskRegEx);
}

bool isNumber(const std::string& sizeAsStr) {
    for (char ch : sizeAsStr) {
        if (!std::isdigit(ch)) {
            return false;
        }
    }
    return true;
}

FileSize fileSizeToBytes(const std::string& sizeAsStr) {
    FileSize sizeInBytes;

    char suffix = sizeAsStr.back();
    if (std::isdigit(suffix)) {
        try {
            sizeInBytes = boost::lexical_cast<FileSize>(sizeAsStr);
        } catch (boost::bad_lexical_cast&) {
            throw std::invalid_argument("the file size must be integer number.");
        }
    } else {
        auto numberAsStr = sizeAsStr.substr(0, sizeAsStr.size() - 1);
        try {
            sizeInBytes = boost::lexical_cast<FileSize>(numberAsStr);
        } catch (boost::bad_lexical_cast&) {
            throw std::invalid_argument("the file size must be integer number.");
        }

        switch (suffix) {
        case SIZE_SUFFIX_KiB:
            sizeInBytes *= BYTES_TO_KiB;
            break;
        case SIZE_SUFFIX_MiB:
            sizeInBytes *= BYTES_TO_MiB;
            break;
        case SIZE_SUFFIX_GiB:
            sizeInBytes *= BYTES_TO_GiB;
            break;
        default:
            using namespace std::string_literals;
            throw std::invalid_argument("unsupported suffix '"s + suffix + "'."s);
        }
    }
    return sizeInBytes;
}

};
