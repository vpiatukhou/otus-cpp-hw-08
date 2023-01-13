#include "Util.h"

#include "gtest/gtest.h"

#include <tuple>

using namespace Homework;

using StrFileSizeParam = std::pair<const char*, FileSize>;

class MatchesWithMaskPositiveTest : public testing::TestWithParam<std::string> {
};

class MatchesWithMaskNegativeTest : public testing::TestWithParam<std::string> {
};

class FileSizeToBytesPositiveTest : public testing::TestWithParam<StrFileSizeParam> {
};

class FileSizeToBytesNegativeTest : public testing::TestWithParam<std::string> {
};

INSTANTIATE_TEST_SUITE_P(UtilTest, MatchesWithMaskPositiveTest, testing::Values("abc", "a*c", "a*", "abc*"));

INSTANTIATE_TEST_SUITE_P(UtilTest, MatchesWithMaskNegativeTest, testing::Values("a*d", "^abc", "abc$", "a.c"));

INSTANTIATE_TEST_SUITE_P(UtilTest, FileSizeToBytesPositiveTest, testing::Values(
    StrFileSizeParam("15", 15),
    StrFileSizeParam("15k", 15 * 1024),
    StrFileSizeParam("15m", 15 * 1024 * 1024),
    StrFileSizeParam("15g", static_cast<FileSize>(15) * 1024 * 1024 * 1024)));

INSTANTIATE_TEST_SUITE_P(UtilTest, FileSizeToBytesNegativeTest, testing::Values("10kk", "10b", "999999999999999999999999"));

TEST_P(MatchesWithMaskPositiveTest, matchesWithMaskPositive) {
    bool result = matchesWithMask("abc", GetParam());
    ASSERT_TRUE(result);
}

TEST_P(MatchesWithMaskNegativeTest, matchesWithMaskNegative) {
    bool result = matchesWithMask("abc", GetParam());
    ASSERT_FALSE(result);
}

TEST_P(FileSizeToBytesPositiveTest, fileSizeToBytesPositive) {
    auto param = GetParam();
    auto result = fileSizeToBytes(param.first);
    ASSERT_EQ(result, param.second);
}

TEST_P(FileSizeToBytesNegativeTest, fileSizeToBytesNotNumber) {
    EXPECT_THROW(fileSizeToBytes(GetParam()), std::invalid_argument);
}
