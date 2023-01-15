#include "ConversionUtil.h"

#include "gtest/gtest.h"

#include <tuple>

using namespace Homework;

using InputAndExpectedValues = std::pair<std::string, FileSize>;

class FileSizeToBytesPositiveTest : public testing::TestWithParam<InputAndExpectedValues> {
};

class FileSizeToBytesNegativeTest : public testing::TestWithParam<std::string> {
};

INSTANTIATE_TEST_SUITE_P(ConversionUtilTest, FileSizeToBytesPositiveTest, testing::Values(
    InputAndExpectedValues("15", 15),
    InputAndExpectedValues("15k", 15 * 1024),
    InputAndExpectedValues("15m", 15 * 1024 * 1024),
    InputAndExpectedValues("15g", static_cast<FileSize>(15) * 1024 * 1024 * 1024)));

INSTANTIATE_TEST_SUITE_P(ConversionUtilTest, FileSizeToBytesNegativeTest, testing::Values("10kk", "10b", "999999999999999999999999"));

TEST_P(FileSizeToBytesPositiveTest, fileSizeToBytesPositive) {
    //given
    auto param = GetParam();

    //when
    auto result = fileSizeToBytes(param.first);

    //then
    ASSERT_EQ(result, param.second);
}

TEST_P(FileSizeToBytesNegativeTest, fileSizeToBytesNotNumber) {
    EXPECT_THROW(fileSizeToBytes(GetParam()), std::invalid_argument);
}
