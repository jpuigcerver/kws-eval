#include <gtest/gtest.h>

#include "core/DocumentBoundingBoxEventSet.h"
#include "core/ShapedEvent.h"

using kws::core::DocumentBoundingBox;
using kws::core::EventSet;
using kws::core::ShapedEvent;

typedef ShapedEvent<int, DocumentBoundingBox<int>> DummyDocEvent;

TEST(DocumentBoundingBoxEventSetTest, Tests) {
  DummyDocEvent e1(1, DocumentBoundingBox<int>("d1", 1, 1, 1, 1));
  DummyDocEvent e2(1, DocumentBoundingBox<int>("d1", 2, 2, 2, 2));
  DummyDocEvent e3(1, DocumentBoundingBox<int>("d2", 2, 2, 2, 2));
  DummyDocEvent e4(2, DocumentBoundingBox<int>("d1", 2, 2, 2, 2));

  EventSet<DummyDocEvent> s;
  EXPECT_EQ(0, s.Size());
  // Insert new event to the multiset
  s.Insert(e1);
  EXPECT_EQ(1, s.Size());
  // Same query, same document, different location: remove does nothing
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
  // Add an event from a different query and document
  s.Insert(e3);
  s.Insert(e4);
  EXPECT_EQ(3, s.Size());
  // Remove e3
  s.Remove(e3);
  EXPECT_EQ(2, s.Size());
  // Make sure e2 and e4 are still in the set
  s.Insert(e2);
  s.Insert(e4);
  EXPECT_EQ(2, s.Size());
  // Clear
  s.Clear();
  EXPECT_EQ(0, s.Size());
}
