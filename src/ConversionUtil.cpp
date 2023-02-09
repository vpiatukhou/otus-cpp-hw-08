#include "ConversionUtil.h"

#include <boost/lexical_cast.hpp>

#include <limits>

namespace Homework {

const char SIZE_SUFFIX_KiB = 'k';
const char SIZE_SUFFIX_MiB = 'm';
const char SIZE_SUFFIX_GiB = 'g';

const FileSize BYTES_TO_KiB = 1024;
const FileSize BYTES_TO_MiB = 1024 * 1024;
const FileSize BYTES_TO_GiB = 1024 * 1024 * 1024;

const FileSize FILE_SIZE_MAX = std::numeric_limits<FileSize>::max();

FileSize stringToFileSize(const std::string& sizeAsStr) {
    FileSize result;
    try {
        result = boost::lexical_cast<FileSize>(sizeAsStr);
    } catch (boost::bad_lexical_cast&) {
        throw std::invalid_argument("the value is not a number or greater than " + std::to_string(FILE_SIZE_MAX) + " bytes.");
    }
    if (result == 0) {
        throw std::invalid_argument("the size must be greater than zero.");
    }
    return result;
}

FileSize fileSizeToBytes(FileSize sizeInBytes, FileSize factor) {
    //test for overflow
    FileSize result = sizeInBytes * factor;
    if (result / factor != sizeInBytes) {
        throw std::invalid_argument("the size must be smaller or equal than " + std::to_string(FILE_SIZE_MAX) + " bytes.");
    }
    return result;
}

FileSize fileSizeToBytes(const std::string& sizeAsStr) {
    FileSize sizeInBytes;

    char suffix = sizeAsStr.back();
    if (std::isdigit(suffix)) {
        sizeInBytes = stringToFileSize(sizeAsStr);
    } else {
        auto numberAsStr = sizeAsStr.substr(0, sizeAsStr.size() - 1);
        sizeInBytes = stringToFileSize(numberAsStr);

        switch (suffix) {
        case SIZE_SUFFIX_KiB:
            sizeInBytes = fileSizeToBytes(sizeInBytes, BYTES_TO_KiB);
            break;
        case SIZE_SUFFIX_MiB:
            sizeInBytes = fileSizeToBytes(sizeInBytes, BYTES_TO_MiB);
            break;
        case SIZE_SUFFIX_GiB:
            sizeInBytes = fileSizeToBytes(sizeInBytes, BYTES_TO_GiB);
            break;
        default:
            using namespace std::string_literals;
            throw std::invalid_argument("unsupported suffix '"s + suffix + "'."s);
        }
    }
    return sizeInBytes;
}

std::size_t stringToSize(const std::string& value) {
    try {
        return boost::lexical_cast<std::size_t>(value);
    } catch (boost::bad_lexical_cast&) {
        throw std::invalid_argument("the value is not a number.");
    }
}

};
