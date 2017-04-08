#include <gtest/gtest.h>

#include "core/DummyLocation.h"
#include "core/EventSet.h"
#include "core/Event.h"

using kws::core::testing::DummyLocation;
using kws::core::EventSet;
using kws::core::Event;

typedef Event<int, DummyLocation> DummyDocEvent;

TEST(EventSetTest, Tests) {
  DummyDocEvent e1(1, DummyLocation(1));
  DummyDocEvent e2(1, DummyLocation(2));
  DummyDocEvent e3(2, DummyLocation(1));

  EventSet<DummyDocEvent> s;
  EXPECT_EQ(0, s.Size());
  // Insert new event to the multiset
  s.Insert(e1);
  EXPECT_EQ(1, s.Size());
  // Same query, different location: remove does nothing
  s.Remove(e2);
  EXPECT_EQ(1, s.Size());
  // Add the same event: it's a set, keep only one copy
  s.Insert(e1);
  EXPECT_EQ(1, s.Size());
  // Add a different event
  s.Insert(e2);
  EXPECT_EQ(2, s.Size());
  // Effective remove first element
  s.Remove(e1);
  EXPECT_EQ(1, s.Size());
  // Make sure that e2 is still in the set
  s.Insert(e2);
  EXPECT_EQ(1, s.Size());
  // Add an event from a different query
  s.Insert(e3);
  EXPECT_EQ(2, s.Size());
  // Remove e3
  s.Remove(e3);
  EXPECT_EQ(1, s.Size());
  // Make sure e2 is still in the set
  s.Insert(e2);
  EXPECT_EQ(1, s.Size());
  // Clear
  s.Clear();
  EXPECT_EQ(0, s.Size());
}
