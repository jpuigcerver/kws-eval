#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/Assessment.h"
#include "core/DummyLocation.h"
#include "core/Event.h"
#include "core/Match.h"
#include "core/ScoredEvent.h"

using kws::core::CollapseMatches;
using kws::core::ComputePrecisionAndRecall;
using kws::core::Event;
using kws::core::Match;
using kws::core::MatchError;
using kws::core::MatchErrorCounts;
using kws::core::ScoredEvent;
using kws::core::testing::DummyLocation;

using testing::IsEmpty;
using testing::ElementsAre;

typedef Event<int, DummyLocation> RefEvent;
typedef ScoredEvent<RefEvent> HypEvent;
typedef Match<RefEvent, HypEvent> DummyMatch;

TEST(AssessmentTest, CollapseMatches) {
  // Empty matches
  {
    const auto output = CollapseMatches(std::vector<DummyMatch>{});
    EXPECT_THAT(output, IsEmpty());
  }
  // Collapse into one
  {
    const std::vector<DummyMatch> matches{
        DummyMatch::MakeFalseNegative(RefEvent()),
        DummyMatch::MakeFalseNegative(RefEvent()),
        DummyMatch::MakeFalseNegative(RefEvent()),
        DummyMatch::MakeFalseNegative(RefEvent())
    };
    const auto output = CollapseMatches(matches);
    EXPECT_THAT(output, ElementsAre(
        MatchErrorCounts(0.0, 4.0, 0, 4)
    ));
  }
  // Generic case
  {
    const std::vector<DummyMatch> matches{
        DummyMatch(RefEvent(), HypEvent(0.5), MatchError(0.0, 0.0)),
        DummyMatch(RefEvent(), HypEvent(0.5), MatchError(0.2, 0.3)),
        DummyMatch::MakeFalsePositive(HypEvent(0.5)),
        DummyMatch::MakeFalsePositive(HypEvent(0.4)),
        DummyMatch::MakeFalseNegative(RefEvent()),
        DummyMatch::MakeFalseNegative(RefEvent())
    };
    const auto output = CollapseMatches(matches);
    EXPECT_THAT(output, ElementsAre(
        MatchErrorCounts(1.2, 0.3, 3, 2),
        MatchErrorCounts(1.0, 0.0, 1, 0),
        MatchErrorCounts(0.0, 2.0, 0, 2)
    ));
  }
}

TEST(AssessmentTest, ComputePrecisionAndRecall) {
  // No references, no hypotheses
  {
    std::vector<float> pr, rc;
    ComputePrecisionAndRecall(
        std::vector<MatchError>{},
        false, &pr, &rc);
    EXPECT_THAT(pr, IsEmpty());
    EXPECT_THAT(rc, IsEmpty());
  }
  // Some references, no hypotheses
  {
    std::vector<float> pr, rc;
    ComputePrecisionAndRecall(
        std::vector<MatchError>{MatchError{0.0f, 1.0f}},
        false, &pr, &rc);
    EXPECT_EQ(1, pr.size());
    EXPECT_EQ(1, rc.size());
    EXPECT_FLOAT_EQ(1.0f, pr[0]);
    EXPECT_FLOAT_EQ(0.0f, rc[0]);
  }
  // Zero references and some hypotheses
  {
    std::vector<float> pr, rc;
    ComputePrecisionAndRecall(
        std::vector<MatchError>{MatchError{1.0f, 0.0f}},
        false, &pr, &rc);
    EXPECT_EQ(1, pr.size());
    EXPECT_EQ(1, rc.size());
    EXPECT_FLOAT_EQ(0.0f, pr[0]);
    EXPECT_FLOAT_EQ(1.0f, rc[0]);
  }
  {
    std::vector<float> pr, rc;
    std::vector<MatchError> m{
      MatchError(0.0f, 0.0f),  // perfect hit, TD = TR = 1
      MatchError(0.2f, 0.0f),  // bbox much bigger than reference, TD = TR = 2
      MatchError(1.0f, 0.0f),  // completely false positive, TD = 3, TR = 2
      MatchError(0.0f, 0.0f),  // another perfect match, TD = 4, TR = 3
      MatchError(0.8f, 0.7f),  // mostly false pos. & neg., TD = 5, TR = 4
      MatchError(0.0f, 1.0f),  // missed reference, TD = 5, TR = 5
    };
    ComputePrecisionAndRecall(m, false, &pr, &rc);
    // Check precision
    EXPECT_EQ(6, pr.size());
    EXPECT_FLOAT_EQ(1.0f / 1.0f, pr[0]);
    EXPECT_FLOAT_EQ(1.8f / 2.0f, pr[1]);
    EXPECT_FLOAT_EQ(1.8f / 3.0f, pr[2]);
    EXPECT_FLOAT_EQ(2.8f / 4.0f, pr[3]);
    EXPECT_FLOAT_EQ(3.0f / 5.0f, pr[4]);
    EXPECT_FLOAT_EQ(3.0f / 5.0f, pr[5]);
    // Check recall
    EXPECT_EQ(6, rc.size());
    EXPECT_FLOAT_EQ(1.0f / 5.0f, rc[0]);
    EXPECT_FLOAT_EQ(2.0f / 5.0f, rc[1]);
    EXPECT_FLOAT_EQ(2.0f / 5.0f, rc[2]);
    EXPECT_FLOAT_EQ(3.0f / 5.0f, rc[3]);
    EXPECT_FLOAT_EQ(3.3f / 5.0f, rc[4]);
    EXPECT_FLOAT_EQ(3.3f / 5.0f, rc[5]);
  }
}
