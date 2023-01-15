#include "ConversionUtil.h"

#include <boost/lexical_cast.hpp>

namespace Homework {

const char SIZE_SUFFIX_KiB = 'k';
const char SIZE_SUFFIX_MiB = 'm';
const char SIZE_SUFFIX_GiB = 'g';

const FileSize BYTES_TO_KiB = 1024;
const FileSize BYTES_TO_MiB = 1024 * 1024;
const FileSize BYTES_TO_GiB = 1024 * 1024 * 1024;

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
