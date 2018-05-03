#include <gtest/gtest.h>

#include "mapper/StringToIntMapper.h"

using kws::mapper::StringToIntMapper;

TEST(StringToIntMapper, DefaultConstructor) {
  StringToIntMapper<int> m;
}

TEST(StringToIntMapper, Simple) {
  std::unordered_map<std::string, int> ht;
  StringToIntMapper<int> m(&ht);
  EXPECT_EQ(m("s1"), 0);
  EXPECT_EQ(m("s2"), 1);
  EXPECT_EQ(m("s1"), 0);
  EXPECT_EQ(m("s3"), 2);
}

TEST(StringToIntMapper, AssertionFailed) {
  std::unordered_map<std::string, int8_t> ht;
  StringToIntMapper<int8_t> m(&ht);
  for (int8_t i = 0; i <= 126; ++i) {
    EXPECT_EQ(m("s" + std::to_string(i)), i);
  }
#ifndef NDEBUG
  EXPECT_DEATH(m("sssss"), "Assertion .* failed.");
#endif
}
