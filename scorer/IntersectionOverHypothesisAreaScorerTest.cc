#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/MockEvent.h"
#include "scorer/IntersectionOverHypothesisAreaScorer.h"

using kws::core::testing::MockEvent;
using kws::scorer::IntersectionOverHypothesisAreaScorer;

using testing::Return;
using testing::_;

typedef IntersectionOverHypothesisAreaScorer<MockEvent, MockEvent>::Result
Result;

TEST(IntersectionOverHypothesisAreaScorerTest, All) {
  IntersectionOverHypothesisAreaScorer<MockEvent, MockEvent> scorer(0.5);
  EXPECT_EQ(scorer.Threshold(), 0.5);

  // Null hypothesis area
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(0));
    EXPECT_EQ(scorer(ref, hyp), Result(0.0f, 1.0f, 1.0f));
  }
  // Overlap is 0%
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), Result(0.0f, 1.0f, 1.0f));
  }
  // Overlap is 30%
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(3));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), Result(0.0f, 1.0f, 1.0f));
  }
  // Overlap is 50%
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(5));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), Result(1.0f, 0.0f, 0.0f));
  }
  // Overlap is 100%
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), Result(1.0f, 0.0f, 0.0f));
  }
}
