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
using kws::core::ComputeAP;
using kws::core::ComputeNDCG;

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

TEST(AssessmentTest, ComputeAP) {
  // Perfect AP
  EXPECT_FLOAT_EQ(
      1.0,
      ComputeAP(std::vector<double>{1.0}, std::vector<double>{1.0}, false));
  // Worst AP
  EXPECT_FLOAT_EQ(
      0.0,
      ComputeAP(std::vector<double>{0.0}, std::vector<double>{0.0}, false));
  EXPECT_FLOAT_EQ(
      1.0 * 0.2 + 0.8 * 0.2 + 0.3 * 0.5,
      ComputeAP(std::vector<double>{1.0, 0.7, 0.8, 0.3},
                std::vector<double>{0.2, 0.2, 0.4, 0.9}, false));
  // Trapezoid interpolation
  EXPECT_FLOAT_EQ(
      1.0 * 0.2 + 0.75 * 0.2 + 0.55 * 0.5,
      ComputeAP(std::vector<double>{1.0, 0.7, 0.8, 0.3},
                std::vector<double>{0.2, 0.2, 0.4, 0.9}, true));
}

TEST(AssessmentTest, ComputeNDCG) {
  // NDCG, empty errors
  EXPECT_FLOAT_EQ(0.0, ComputeNDCG<double>(std::vector<MatchError>{}));
  // Perfect NDCG
  EXPECT_FLOAT_EQ(
      1.0,
      ComputeNDCG<double>(std::vector<MatchError>{MatchError(0.0f, 0.0f)}));
  // Worst NDCG
  EXPECT_FLOAT_EQ(
      0.0,
      ComputeNDCG<double>(std::vector<MatchError>{MatchError(0.0f, 1.0f)}));
  // Generic NDCG
  const double DCG =
      1.0 / log2(2.0) +
      (pow(2.0, 0.8) - 1) / log2(3.0) +
      1.0 / log2(5.0) +
      (pow(2.0, 0.2) - 1) / log2(6.0);
  const double Z =
      1.0 / log2(2.0) +
      1.0 / log2(3.0) +
      1.0 / log2(4.0) +
      1.0 / log2(5.0) +
      1.0 / log2(6.0);
  EXPECT_FLOAT_EQ(
      DCG / Z,
      ComputeNDCG<double>(std::vector<MatchError>{
          MatchError(0.0f, 0.0f),  // perfect hit, TD = TR = 1
          MatchError(0.2f, 0.0f),  // bbox bigger than reference, TD = TR = 2
          MatchError(1.0f, 0.0f),  // completely false positive, TD = 3, TR = 2
          MatchError(0.0f, 0.0f),  // another perfect match, TD = 4, TR = 3
          MatchError(0.8f, 0.7f),  // mostly false pos. & neg., TD = 5, TR = 4
          MatchError(0.0f, 1.0f),  // missed reference, TD = 5, TR = 5
      }));
  // Pefect NDCG with collapsed events
  EXPECT_FLOAT_EQ(
      1.0,
      ComputeNDCG<double>(std::vector<MatchErrorCounts>{
        MatchErrorCounts(0.0f, 0.0f, 4, 4)}));
  // NDCG, all missing
  EXPECT_FLOAT_EQ(0.0, ComputeNDCG<double>(std::vector<MatchErrorCounts>{
      MatchErrorCounts(0.0f, 1.0f, 0, 5)
  }));
  // NDCG, all false positive
  EXPECT_FLOAT_EQ(0.0, ComputeNDCG<double>(std::vector<MatchErrorCounts>{
      MatchErrorCounts(1.0f, 1.0f, 9, 4)
  }));
  // Generic NDCG collapsed events
  EXPECT_FLOAT_EQ(
      (1 / log2(2) +
          (pow(2.0, 0.8) - 1) / log2(3) +
          (pow(2.0, 0.25) - 1) / log2(4)) /
          (1 / log2(2) + 1 / log2(3) + 1 / log2(4)),
      ComputeNDCG<double>(std::vector<MatchErrorCounts>{
          MatchErrorCounts(0.0f, 0.0f, 1, 1),
          MatchErrorCounts(0.2f, 0.0f, 5, 5),
          MatchErrorCounts(0.75f, 0.5f, 4, 2),
  }));
}
