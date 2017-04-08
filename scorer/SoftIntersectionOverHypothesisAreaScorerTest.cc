#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/MockEvent.h"
#include "scorer/SoftIntersectionOverHypothesisAreaScorer.h"

using kws::core::testing::MockEvent;
using kws::core::MatchError;
using kws::scorer::SoftIntersectionOverHypothesisAreaScorer;

using testing::Return;
using testing::_;

TEST(SoftIntersectionOverHypothesisAreaScorerTest, All) {
  SoftIntersectionOverHypothesisAreaScorer<MockEvent, MockEvent> scorer;

  // Overlap is 0, Hypothesis is 0 (both areas area 0)
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
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(0));
    EXPECT_EQ(scorer(ref, hyp), MatchError(1.0f, 1.0f));
  }
  // Overlap is 0, ref area is 0, hyp area is >0
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(0));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), MatchError(1.0f, 1.0f));
  }
  // Overlap is 0, ref area is >0, hyp area is >0
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(5));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(5));
    EXPECT_EQ(scorer(ref, hyp), MatchError(1.0f, 1.0f));
  }
  // Perfect match
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), MatchError(0.0f, 0.0f));
  }
  // Has false positive area
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(12));
    const auto s = scorer(ref, hyp);
    EXPECT_FLOAT_EQ(s.FP(), 2.0f / 12.0f);
    EXPECT_FLOAT_EQ(s.FN(), 0.0f);
  }
  // Has false negative area
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(12));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    const auto s = scorer(ref, hyp);
    EXPECT_FLOAT_EQ(s.FP(), 0.0f);
    EXPECT_FLOAT_EQ(s.FN(), 2.0f / 12.0f);
  }
  // Has both false positive and false negative area
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(4));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(8));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(6));
    const auto s = scorer(ref, hyp);
    EXPECT_FLOAT_EQ(s.FP(), 2.0f / 6.0f);
    EXPECT_FLOAT_EQ(s.FN(), 4.0f / 8.0f);
  }
}
