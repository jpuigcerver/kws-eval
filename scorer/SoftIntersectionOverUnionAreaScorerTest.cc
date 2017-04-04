#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/MockEvent.h"
#include "scorer/SoftIntersectionOverUnionAreaScorer.h"

using kws::core::testing::MockEvent;
using kws::scorer::SoftIntersectionOverUnionAreaScorer;

using testing::Return;
using testing::_;

typedef SoftIntersectionOverUnionAreaScorer<MockEvent, MockEvent>::Result
Result;

TEST(SoftIntersectionOverUnionAreaScorerTest, All) {
  SoftIntersectionOverUnionAreaScorer<MockEvent, MockEvent> scorer;

  // Overlap is 0, union is 0 (both areas area 0)
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(0));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(0));
    EXPECT_EQ(scorer(ref, hyp), Result(0.0f, 1.0f, 1.0f));
  }
  // Overlap is 0, ref area is >0, hyp area is 0
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(0));
    EXPECT_EQ(scorer(ref, hyp), Result(0.0f, 1.0f, 1.0f));
  }
  // Overlap is 0, ref area is 0, hyp area is >0
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(0));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    EXPECT_EQ(scorer(ref, hyp), Result(0.0f, 1.0f, 1.0f));
  }
  // Overlap is 0, ref area is >0, hyp area is >0
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(5));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(5));
    EXPECT_EQ(scorer(ref, hyp), Result(0.0f, 1.0f, 1.0f));
  }
  // Perfect match
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    const Result s = scorer(ref, hyp);
    EXPECT_FLOAT_EQ(s.tp, 1.0f);
    EXPECT_FLOAT_EQ(s.fp, 0.0f);
    EXPECT_FLOAT_EQ(s.fn, 0.0f);
  }
  // Has false positive area
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(12));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(10));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(12));
    const Result s = scorer(ref, hyp);
    EXPECT_FLOAT_EQ(s.tp, 10.0f / 12.0f);
    EXPECT_FLOAT_EQ(s.fp, 2.0f / 12.0f);
    EXPECT_FLOAT_EQ(s.fn, 0.0f);
  }
  // Has false negative area
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(12));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(12));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(10));
    const Result s = scorer(ref, hyp);
    EXPECT_FLOAT_EQ(s.tp, 10.0f / 12.0f);
    EXPECT_FLOAT_EQ(s.fp, 0.0f);
    EXPECT_FLOAT_EQ(s.fn, 2.0f / 12.0f);
  }
  // Has both false positive and false negative area
  {
    MockEvent ref, hyp;
    EXPECT_CALL(ref, IntersectionArea(_)).WillRepeatedly(Return(3));
    EXPECT_CALL(ref, UnionArea(_)).WillRepeatedly(Return(10));
    EXPECT_CALL(ref, Area()).WillRepeatedly(Return(7));
    EXPECT_CALL(hyp, Area()).WillRepeatedly(Return(6));
    const Result s = scorer(ref, hyp);
    EXPECT_FLOAT_EQ(s.tp, 3.0f / 10.0f);
    EXPECT_FLOAT_EQ(s.fp, 1.0f - 3.0f / 6.0f);
    EXPECT_FLOAT_EQ(s.fn, 1.0f - 3.0f / 7.0f);
  }

}
