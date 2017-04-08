#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/Event.h"
#include "core/MockLocation.h"

using kws::core::Event;
using kws::core::testing::MockLocation;

using testing::Return;
using testing::_;
using testing::Eq;

TEST(EventTest, Comparisons) {
  // Equality operator
  {
    Event<int, MockLocation<int>> e1, e2, e3, e4;
    e1.Query() = e2.Query() = e3.Query() = 1; e4.Query() = 2;
    EXPECT_CALL(e1.Location(), Equals(_))
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_TRUE(e1 == e2);   // Same event
    EXPECT_FALSE(e1 == e3);  // Different location
    EXPECT_FALSE(e1 == e4);  // Different query
  }
  // Inequality operator
  {
    Event<int, MockLocation<int>> e1, e2, e3, e4;
    e1.Query() = e2.Query() = e3.Query() = 1; e4.Query() = 2;
    EXPECT_CALL(e1.Location(), NotEquals(_))
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_FALSE(e1 != e2); // Same event
    EXPECT_TRUE(e1 != e3);  // Different location
    EXPECT_TRUE(e1 != e4);  // Different query
  }
  // Lower operator
  {
    Event<int, MockLocation<int>> e1, e2, e3, e4;
    e1.Query() = e2.Query() = e3.Query() = 1; e4.Query() = 2;
    EXPECT_CALL(e1.Location(), Lower(_))
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(e3.Location(), Lower(_))
        .WillOnce(Return(false));
    EXPECT_FALSE(e1 < e2);  // Same event
    EXPECT_TRUE(e1 < e3);   // Same query, lower location
    EXPECT_TRUE(e1 < e4);   // Lower query
    EXPECT_FALSE(e3 < e1);  // Same query, lower location
    EXPECT_FALSE(e4 < e1);
  }
  // Greater operator
  {
    Event<int, MockLocation<int>> e1, e2, e3, e4;
    e1.Query() = e2.Query() = e3.Query() = 1; e4.Query() = 2;
    EXPECT_CALL(e1.Location(), Lower(_))
        .WillOnce(Return(false));
    EXPECT_CALL(e2.Location(), Lower(_))
        .WillOnce(Return(false));
    EXPECT_CALL(e3.Location(), Lower(_))
        .WillOnce(Return(true));
    EXPECT_FALSE(e1 > e2);  // Same event
    EXPECT_TRUE(e1 > e3);   // Same query, greater location
    EXPECT_FALSE(e1 > e4);  // e1 has lower query than e4
    EXPECT_TRUE(e4 > e1);   // e1 has lower query than e4
    EXPECT_FALSE(e3 > e1);
  }
}

TEST(EventTest, Geometry) {
  // Event area
  {
    Event<int, MockLocation<int>> e1;
    EXPECT_CALL(e1.Location(), Area()).WillRepeatedly(Return(99));
    EXPECT_EQ(e1.Area(), e1.Location().Area());
  }
  // Intersection area
  {
    Event<int, MockLocation<int>> e1, e2;
    EXPECT_CALL(e1.Location(), IntersectionArea(_)).WillRepeatedly(Return(99));
    EXPECT_EQ(IntersectionArea(e1, e2), 99);
  }
  // Union area
  {
    Event<int, MockLocation<int>> e1, e2;
    EXPECT_CALL(e1.Location(), UnionArea(_)).WillRepeatedly(Return(99));
    EXPECT_EQ(UnionArea(e1, e2), 99);
  }
}

TEST(EventTest, Streams) {
  Event<int, MockLocation<int>> e1, e2;
  e1.Query() = 1;
  // Output stream
  std::ostringstream oss;
  oss << e1;
  EXPECT_EQ(oss.str(), "1 " + e1.Location().String());
  // Input stream
  std::istringstream iss("1 " + e1.Location().String());
  iss >> e2;
  EXPECT_EQ(e2.Query(), e1.Query());
  EXPECT_EQ(e2.Location().String(), e1.Location().String());
}
