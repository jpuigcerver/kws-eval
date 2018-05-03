#include <gtest/gtest.h>

#include "core/Event.h"
#include "mapper/StringEventToIntMapper.h"

using kws::core::Event;
using kws::mapper::StringToIntMapper;
using kws::mapper::StringEventToIntMapper;

TEST(StringEventToIntMapper, Constructor) {
  StringEventToIntMapper<int> m1;
  StringEventToIntMapper<int> m2(m1);
  StringToIntMapper<int> sm;
  StringEventToIntMapper<int> m3(&sm);
  StringEventToIntMapper<int> m4(&sm, false);
  StringEventToIntMapper<int> m5(new StringToIntMapper<int>(), true);
}

TEST(StringEventToIntMapper, GetMapper) {
  StringEventToIntMapper<int> m1;
  EXPECT_NE(m1.GetMapper(), nullptr);

  StringToIntMapper<int> sm;
  StringEventToIntMapper<int> m2(&sm);
  EXPECT_EQ(m2.GetMapper(), &sm);
}

TEST(StringEventToIntMapper, Simple) {
  typedef StringEventToIntMapper<int>::InputType InputEvent;
  typedef StringEventToIntMapper<int>::OutputType OutputEvent;

  std::unordered_map<std::string, int> ht;
  StringToIntMapper<int> sm(&ht);
  StringEventToIntMapper<int> em(&sm);

  EXPECT_EQ(em(InputEvent("foo", "foo")), OutputEvent(0, 0));
  EXPECT_EQ(em(InputEvent("foo", "bar")), OutputEvent(0, 1));
  EXPECT_EQ(em(InputEvent("bar", "foo")), OutputEvent(1, 0));
}
