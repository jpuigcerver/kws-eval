#include <gtest/gtest.h>

#include "core/BoundingBox.h"

using kws::core::BoundingBox;

TEST(BoundingBoxTest, Constructor) {
  // Default constructor
  BoundingBox<int> b1;
  b1.x = 1;
  b1.y = 2;
  b1.w = 3;
  b1.h = 4;
  EXPECT_EQ(b1.x, 1);
  EXPECT_EQ(b1.y, 2);
  EXPECT_EQ(b1.w, 3);
  EXPECT_EQ(b1.h, 4);

  // Explicit constructor
  BoundingBox<int> b2(1, 2, 3, 4);
  EXPECT_EQ(b2.x, 1);
  EXPECT_EQ(b2.y, 2);
  EXPECT_EQ(b2.w, 3);
  EXPECT_EQ(b2.h, 4);

  // Copy constructor
  BoundingBox<int> b3(b2);
  EXPECT_EQ(b3.x, 1);
  EXPECT_EQ(b3.y, 2);
  EXPECT_EQ(b3.w, 3);
  EXPECT_EQ(b3.h, 4);

  EXPECT_EQ(b1.Area(), 12);
  EXPECT_EQ(b2.Area(), 12);
  EXPECT_EQ(b3.Area(), 12);
}

TEST(BoundingBoxTest, Comparison) {
  BoundingBox<int> b1(1, 2, 3, 4);
  BoundingBox<int> b2(1, 2, 3, 5);
  BoundingBox<int> b3(b2);
  EXPECT_NE(b1, b2);  // b1 != b2
  EXPECT_EQ(b2, b3);  // b2 == b3
  EXPECT_LT(b1, b2);  // b1.h < b2.h
  EXPECT_GT(b2, b1);  //
  EXPECT_LE(b1, b2);  // b1.h < b2.h
  EXPECT_LE(b2, b3);  //
  EXPECT_GE(b2, b1);  //
  EXPECT_GE(b2, b3);  //
}

TEST(BoundingBoxTest, Geometry) {
  // T = int
  {
    BoundingBox<int> b0(4, 4, 0, 0);
    BoundingBox<int> b1(0, 1, 4, 5);
    BoundingBox<int> b2(b1);
    BoundingBox<int> b3(99, 99, 99, 99);
    BoundingBox<int> b4(-1, 0, 3, 4);

    // Intersection area
    EXPECT_EQ(b1.IntersectionArea(b0), 0);
    EXPECT_EQ(b1.IntersectionArea(b2), b1.Area());
    EXPECT_EQ(b1.IntersectionArea(b3), 0);
    EXPECT_EQ(b1.IntersectionArea(b4), 6);
    EXPECT_EQ(b1.IntersectionArea(b4), kws::core::IntersectionArea(b1, b4));

    // Union Area
    EXPECT_EQ(b1.UnionArea(b0), b1.Area());
    EXPECT_EQ(b1.UnionArea(b1), b1.Area());
    EXPECT_EQ(b1.UnionArea(b2), b1.Area());
    EXPECT_EQ(b1.UnionArea(b3), b1.Area() + b3.Area());
    EXPECT_EQ(b1.UnionArea(b4), b1.Area() + 6);
    EXPECT_EQ(b1.UnionArea(b4), kws::core::UnionArea(b1, b4));
  }

  // T = unsigned int
  {
    BoundingBox<unsigned int> b0(4, 4, 0, 0);
    BoundingBox<unsigned int> b1(0, 1, 4, 5);
    BoundingBox<unsigned int> b2(b1);
    BoundingBox<unsigned int> b3(99, 99, 99, 99);
    BoundingBox<unsigned int> b4(2, 3, 3, 4);

    // Intersection area
    EXPECT_EQ(b1.IntersectionArea(b0), 0);
    EXPECT_EQ(b1.IntersectionArea(b2), b1.Area());
    EXPECT_EQ(b1.IntersectionArea(b3), 0);
    EXPECT_EQ(b1.IntersectionArea(b4), 6);
    EXPECT_EQ(b1.IntersectionArea(b4), kws::core::IntersectionArea(b1, b4));

    // Union Area
    EXPECT_EQ(b1.UnionArea(b0), b1.Area());
    EXPECT_EQ(b1.UnionArea(b1), b1.Area());
    EXPECT_EQ(b1.UnionArea(b2), b1.Area());
    EXPECT_EQ(b1.UnionArea(b3), b1.Area() + b3.Area());
    EXPECT_EQ(b1.UnionArea(b4), b1.Area() + 6);
    EXPECT_EQ(b1.UnionArea(b4), kws::core::UnionArea(b1, b4));
  }
}

TEST(BoundingBoxTest, Streams) {
  BoundingBox<int> b1(1, 2, 3, 4);
  BoundingBox<int> b2;
  // Output stream
  std::ostringstream oss;
  oss << b1;
  EXPECT_EQ(oss.str(), "1 2 3 4");
  // Input stream
  std::istringstream iss("1 2 3 4");
  iss >> b2;
  EXPECT_EQ(b1, b2);
}
