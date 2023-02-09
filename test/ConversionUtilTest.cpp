#include "ConversionUtil.h"

#include "gtest/gtest.h"

#include <tuple>
#include <limits>

using namespace Homework;

const FileSize BYTES_TO_KiB = 1024;
const FileSize BYTES_TO_MiB = 1024 * 1024;
const FileSize BYTES_TO_GiB = 1024 * 1024 * 1024;

const FileSize FILE_SIZE_MAX = std::numeric_limits<FileSize>::max();

using InputAndExpectedValues = std::pair<std::string, FileSize>;

FileSize getExpectedFileSize(FileSize factor) {
    return (FILE_SIZE_MAX / factor) * factor;
}

class FileSizeToBytesPositiveTest : public testing::TestWithParam<InputAndExpectedValues> {
};

class FileSizeToBytesNegativeTest : public testing::TestWithParam<std::string> {
};

INSTANTIATE_TEST_SUITE_P(ConversionUtilTest, FileSizeToBytesPositiveTest, testing::Values(
    InputAndExpectedValues("15", 15),
    InputAndExpectedValues("15k", 15 * BYTES_TO_KiB),
    InputAndExpectedValues("15m", 15 * BYTES_TO_MiB),
    InputAndExpectedValues("15g", 15 * BYTES_TO_GiB),
    InputAndExpectedValues(std::to_string(FILE_SIZE_MAX), FILE_SIZE_MAX),
    InputAndExpectedValues(std::to_string(FILE_SIZE_MAX / BYTES_TO_KiB) + 'k', getExpectedFileSize(BYTES_TO_KiB)),
    InputAndExpectedValues(std::to_string(FILE_SIZE_MAX / BYTES_TO_MiB) + 'm', getExpectedFileSize(BYTES_TO_MiB)),
    InputAndExpectedValues(std::to_string(FILE_SIZE_MAX / BYTES_TO_GiB) + 'g', getExpectedFileSize(BYTES_TO_GiB))));

INSTANTIATE_TEST_SUITE_P(ConversionUtilTest, FileSizeToBytesNegativeTest, testing::Values(
    "0", "0k", "0m", "0g",
    "10kk", "10b", 
    std::to_string(FILE_SIZE_MAX + 1),
    std::to_string((FILE_SIZE_MAX + 1) / BYTES_TO_KiB) + 'k',
    std::to_string((FILE_SIZE_MAX + 1) / BYTES_TO_MiB) + 'm',
    std::to_string((FILE_SIZE_MAX + 1) / BYTES_TO_GiB) + 'g'));

TEST_P(FileSizeToBytesPositiveTest, fileSizeToBytesPositive) {
    //given
    auto param = GetParam();

    //when
    auto result = fileSizeToBytes(param.first);

    //then
    ASSERT_EQ(result, param.second);
}

TEST_P(FileSizeToBytesNegativeTest, fileSizeToBytesNegative) {
    EXPECT_THROW(fileSizeToBytes(GetParam()), std::invalid_argument);
}
