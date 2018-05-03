#include <gtest/gtest.h>

#include "mapper/StringToIntMapper.h"

using kws::mapper::StringToIntMapper;

TEST(StringToIntMapper, Constructors) {
  StringToIntMapper<int> m1;

  std::unordered_map<std::string, int> ht;
  StringToIntMapper<int> m2(&ht);

  StringToIntMapper<int> m3(&ht, false);
  StringToIntMapper<int> m4(new std::unordered_map<std::string, int>, true);
}


TEST(StringToIntMapper, Simple) {
  std::unordered_map<std::string, int> ht;
  StringToIntMapper<int> m(&ht);
  EXPECT_EQ(m("s1"), 0);
  EXPECT_EQ(m("s2"), 1);
  EXPECT_EQ(m("s1"), 0);
  EXPECT_EQ(m("s3"), 2);
}

TEST(StringToIntMapper, RangeErrorException) {
  std::unordered_map<std::string, int8_t> ht;
  StringToIntMapper<int8_t> m(&ht);
  for (uint8_t i = 0; i <= 127; ++i) {
    EXPECT_EQ(m("s" + std::to_string(i)), i);
  }
  EXPECT_THROW(m("except"), std::range_error);
}
