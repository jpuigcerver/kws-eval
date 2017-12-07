#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/MockEvent.h"
#include "scorer/IntersectionOverUnionAreaScorer.h"

using kws::core::testing::MockEvent;
using kws::core::MatchError;
using kws::scorer::IntersectionOverUnionAreaScorer;

using testing::Return;
using testing::_;

TEST(IntersectionOverUnionAreaScorerTest, All) {
  IntersectionOverUnionAreaScorer<MockEvent, MockEvent> scorer(0.5);
  EXPECT_EQ(scorer.Threshold(), 0.5);
  // Overlap is 0, Hypothesis is 0 (both areas are 0)
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(0));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(0));
    EXPECT_EQ(scorer(ref, hyp), MatchError(1.0f, 1.0f));
  }
  // Overlap is 0, ref area is >0, hyp area is 0
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), MatchError(1.0f, 1.0f));
  }
  // Overlap is 0, ref area is 0, ref area is >0
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(0));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), MatchError(1.0f, 1.0f));
  }
  // Overlap is 0%
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(20));
    EXPECT_EQ(scorer(ref, hyp), MatchError(1.0f, 1.0f));
  }
  // IoU is < 50%
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(3));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(17));
    EXPECT_EQ(scorer(ref, hyp), MatchError(1.0f, 1.0f));
  }
  // IoU is 50%
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(12));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(12));
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(8));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(16));
    EXPECT_EQ(scorer(ref, hyp), MatchError(0.0f, 0.0f));
  }
  // IoU > 50%
  {
    MockEvent ref, hyp;
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(8));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(12));
    EXPECT_EQ(scorer(ref, hyp), MatchError(0.0f, 0.0f));
  }
}
