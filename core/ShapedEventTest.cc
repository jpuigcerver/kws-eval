#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/ShapedEvent.h"
#include "core/MockLocation.h"

using kws::core::ShapedEvent;
using kws::core::testing::MockLocation;

using testing::Return;
using testing::_;
using testing::Eq;

TEST(EventTest, Geometry) {
  // Event area
  {
    ShapedEvent<int, MockLocation<int>> e1;
    EXPECT_CALL(e1.Location(), Area()).WillRepeatedly(Return(99));
    EXPECT_EQ(e1.Area(), e1.Location().Area());
  }
  // Intersection area
  {
    ShapedEvent<int, MockLocation<int>> e1, e2;
    EXPECT_CALL(e1.Location(), IntersectionArea(_)).WillRepeatedly(Return(99));
    EXPECT_EQ(IntersectionArea(e1, e2), 99);
  }
  // Union area
  {
    ShapedEvent<int, MockLocation<int>> e1, e2;
    EXPECT_CALL(e1.Location(), UnionArea(_)).WillRepeatedly(Return(99));
    EXPECT_EQ(UnionArea(e1, e2), 99);
  }
}

