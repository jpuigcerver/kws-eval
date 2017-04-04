#include <gtest/gtest.h>

#include "core/DocumentBoundingBox.h"

using kws::core::DocumentBoundingBox;

TEST(DocumentBoundingBoxTest, Constructor) {
  // Default constructor
  DocumentBoundingBox<int> b1;
  b1.x = 1;
  b1.y = 2;
  b1.w = 3;
  b1.h = 4;
  b1.document = "doc";
  EXPECT_EQ(b1.x, 1);
  EXPECT_EQ(b1.y, 2);
  EXPECT_EQ(b1.w, 3);
  EXPECT_EQ(b1.h, 4);
  EXPECT_EQ("doc", b1.document);

  // Explicit constructor
  DocumentBoundingBox<int> b2("doc", 1, 2, 3, 4);
  EXPECT_EQ(b2.x, 1);
  EXPECT_EQ(b2.y, 2);
  EXPECT_EQ(b2.w, 3);
  EXPECT_EQ(b2.h, 4);
  EXPECT_EQ(b2.document, "doc");

  // Copy constructor
  DocumentBoundingBox<int> b3(b2);
  EXPECT_EQ(b3.x, 1);
  EXPECT_EQ(b3.y, 2);
  EXPECT_EQ(b3.w, 3);
  EXPECT_EQ(b3.h, 4);
  EXPECT_EQ(b3.document, "doc");

  EXPECT_EQ(b1.Area(), 12);
  EXPECT_EQ(b2.Area(), 12);
  EXPECT_EQ(b3.Area(), 12);
}

TEST(DocumentBoundingBoxTest, Comparison) {
  DocumentBoundingBox<int> b1("doc1", 1, 2, 3, 4);
  DocumentBoundingBox<int> b2("doc1", 1, 2, 3, 5);
  DocumentBoundingBox<int> b3(b2);
  DocumentBoundingBox<int> b4("doc2", 1, 2, 3, 4);
  EXPECT_NE(b1, b2);  // b1 != b2
  EXPECT_EQ(b2, b3);  // b2 == b3
  EXPECT_LT(b1, b2);  // b1.h < b2.h
  EXPECT_GT(b2, b1);  //
  EXPECT_LE(b1, b2);  // b1.h < b2.h
  EXPECT_LE(b2, b3);  //
  EXPECT_GE(b2, b1);  //
  EXPECT_GE(b2, b3);  //
  EXPECT_NE(b1, b4);  // doc1 != doc2
  EXPECT_LT(b1, b4);  // doc1 < doc2
  EXPECT_GT(b4, b1);  // doc2 > doc1
}

TEST(DocumentBoundingBoxTest, Geometry) {
  // T = int
  {
    DocumentBoundingBox<int> b0("doc1", 4, 4, 0, 0);
    DocumentBoundingBox<int> b1("doc1", 0, 1, 4, 5);
    DocumentBoundingBox<int> b2(b1);
    DocumentBoundingBox<int> b3("doc1", 99, 99, 99, 99);
    DocumentBoundingBox<int> b4("doc1", -1, 0, 3, 4);
    DocumentBoundingBox<int> b5("doc2", 0, 1, 4, 5);  // same coord, diff docs

    // Intersection area
    EXPECT_EQ(b1.IntersectionArea(b0), 0);
    EXPECT_EQ(b1.IntersectionArea(b2), b1.Area());
    EXPECT_EQ(b1.IntersectionArea(b3), 0);
    EXPECT_EQ(b1.IntersectionArea(b4), 6);
    EXPECT_EQ(b1.IntersectionArea(b4), kws::core::IntersectionArea(b1, b4));
    EXPECT_EQ(b1.IntersectionArea(b5), 0);

    // Union Area
    EXPECT_EQ(b1.UnionArea(b0), b1.Area());
    EXPECT_EQ(b1.UnionArea(b1), b1.Area());
    EXPECT_EQ(b1.UnionArea(b2), b1.Area());
    EXPECT_EQ(b1.UnionArea(b3), b1.Area() + b3.Area());
    EXPECT_EQ(b1.UnionArea(b4), b1.Area() + 6);
    EXPECT_EQ(b1.UnionArea(b4), kws::core::UnionArea(b1, b4));
    EXPECT_EQ(b1.UnionArea(b5), b1.Area() + b5.Area());
  }

  // T = unsigned int
  {
    DocumentBoundingBox<unsigned int> b0("doc1", 4, 4, 0, 0);
    DocumentBoundingBox<unsigned int> b1("doc1", 0, 1, 4, 5);
    DocumentBoundingBox<unsigned int> b2(b1);
    DocumentBoundingBox<unsigned int> b3("doc1", 99, 99, 99, 99);
    DocumentBoundingBox<unsigned int> b4("doc1", 2, 3, 3, 4);
    DocumentBoundingBox<unsigned int> b5("doc2", 0, 1, 4, 5);

    // Intersection area
    EXPECT_EQ(b1.IntersectionArea(b0), 0);
    EXPECT_EQ(b1.IntersectionArea(b2), b1.Area());
    EXPECT_EQ(b1.IntersectionArea(b3), 0);
    EXPECT_EQ(b1.IntersectionArea(b4), 6);
    EXPECT_EQ(b1.IntersectionArea(b4), kws::core::IntersectionArea(b1, b4));
    EXPECT_EQ(b1.IntersectionArea(b5), 0);

    // Union Area
    EXPECT_EQ(b1.UnionArea(b0), b1.Area());
    EXPECT_EQ(b1.UnionArea(b1), b1.Area());
    EXPECT_EQ(b1.UnionArea(b2), b1.Area());
    EXPECT_EQ(b1.UnionArea(b3), b1.Area() + b3.Area());
    EXPECT_EQ(b1.UnionArea(b4), b1.Area() + 6);
    EXPECT_EQ(b1.UnionArea(b4), kws::core::UnionArea(b1, b4));
    EXPECT_EQ(b1.UnionArea(b5), b1.Area() + b5.Area());
  }
}

TEST(DocumentBoundingBoxTest, Streams) {
  DocumentBoundingBox<int> b1("doc", 1, 2, 3, 4);
  DocumentBoundingBox<int> b2;
  // Output stream
  std::ostringstream oss;
  oss << b1;
  EXPECT_EQ(oss.str(), "doc 1 2 3 4");
  // Input stream
  std::istringstream iss("doc 1 2 3 4");
  iss >> b2;
  EXPECT_EQ(b1, b2);
}
