#include "StringMatcher.h"

#include "gtest/gtest.h"

#include <utility>

using namespace Homework;

using MaskAndStrValues = std::pair<std::string, std::string>;

class StringMatcherPositiveTest : public testing::TestWithParam<MaskAndStrValues> {
};

class StringMatcherNegativeTest : public testing::TestWithParam<MaskAndStrValues> {
};

INSTANTIATE_TEST_SUITE_P(StringMatcherTest, StringMatcherPositiveTest, testing::Values(
    MaskAndStrValues("abc", "abc"),
    MaskAndStrValues("aaa*c*", "aaabbbcccddd"),
    MaskAndStrValues("?b?d?", "abcde"),
    MaskAndStrValues("\\?b\\?d\\?", "?b?d?"),
    MaskAndStrValues("\\??c*", "?bcde"),
    MaskAndStrValues("a\\dc", "a\\dc"),
    MaskAndStrValues("a.c", "a.c"),
    MaskAndStrValues("a[b]c", "a[b]c"),
    MaskAndStrValues("a{1}", "a{1}"),
    MaskAndStrValues("a(b)", "a(b)"),
    MaskAndStrValues("a+", "a+"),
    MaskAndStrValues("^abc$", "^abc$"),
    MaskAndStrValues("a|b", "a|b")));

INSTANTIATE_TEST_SUITE_P(StringMatcherTest, StringMatcherNegativeTest, testing::Values(
    MaskAndStrValues("abc", "abd"),
    MaskAndStrValues("a?", "abc"),
    MaskAndStrValues("a.c", "abc")));

TEST_P(StringMatcherPositiveTest, matchesPositive) {
    //given
    auto params = GetParam();
    std::vector<std::string> masks = { params.first };
    StringMatcher matcher(masks);

    //when
    bool result = matcher.matches(params.second);

    //then
    ASSERT_TRUE(result);
}

TEST_P(StringMatcherNegativeTest, matchesNegative) {
    //given
    auto params = GetParam();
    std::vector<std::string> masks = { params.first };
    StringMatcher matcher(masks);

    //when
    bool result = matcher.matches(params.second);
    
    //then
    ASSERT_FALSE(result);
}
