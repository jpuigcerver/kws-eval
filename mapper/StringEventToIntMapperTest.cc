#include <gtest/gtest.h>

#include "core/Event.h"
#include "mapper/StringEventToIntMapper.h"

using kws::core::Event;
using kws::mapper::StringToIntMapper;
using kws::mapper::StringEventToIntMapper;

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
