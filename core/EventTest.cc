#include <gtest/gtest.h>

#include "core/Event.h"
#include "core/BoundingBox.h"

using kws::core::Event;
using kws::core::BoundingBox;

TEST(EventTest, Constructor) {
  Event<BoundingBox<int>> e1;
  e1.query = "q";
  e1.location = BoundingBox<int>(1, 2, 3, 4);
  Event<BoundingBox<int>> e2(e1);
  EXPECT_EQ(e1.query, e2.query);
  EXPECT_EQ(e1.location, e2.location);
}

TEST(EventTest, Comparison) {
  // TODO
}

TEST(EventTest, Geometry) {
  // TODO
}

TEST(EventTest, Streams) {
  Event<BoundingBox<int>> e1("q1",  BoundingBox<int>(1, 2, 3, 4));
  Event<BoundingBox<int>> e2;
  // Output stream
  std::ostringstream oss;
  oss << e1;
  EXPECT_EQ(oss.str(), "q1 1 2 3 4");
  // Input stream
  std::istringstream iss("q1 1 2 3 4");
  iss >> e2;
  EXPECT_EQ(e1, e2);
}
