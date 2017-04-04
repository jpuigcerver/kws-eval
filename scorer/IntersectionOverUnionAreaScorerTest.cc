#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/MockEvent.h"
#include "scorer/IntersectionOverUnionAreaScorer.h"

using kws::core::testing::MockEvent;
using kws::scorer::IntersectionOverUnionAreaScorer;

using testing::Return;
using testing::_;

typedef IntersectionOverUnionAreaScorer<MockEvent, MockEvent>::Result
Result;

TEST(IntersectionOverUnionAreaScorerTest, All) {
  IntersectionOverUnionAreaScorer<MockEvent, MockEvent> scorer(0.4);
  EXPECT_FLOAT_EQ(scorer.Threshold(), 0.4);

  // Overlap is 0, union is 0
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(0));
    EXPECT_EQ(scorer(ref, hyp), Result(0.0f, 1.0f, 1.0f));
  }
  // Overlap is 0, union is >0
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), Result(0.0f, 1.0f, 1.0f));
  }
  // Overlap is 30%
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(3));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), Result(0.0f, 1.0f, 1.0f));
  }
  // Overlap is 50%
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(5));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), Result(1.0f, 0.0f, 0.0f));
  }
  // Overlap is 100%
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), Result(1.0f, 0.0f, 0.0f));
  }
}
