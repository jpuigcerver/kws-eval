#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/BoundingBox.h"
#include "core/Event.h"
#include "core/ScoredEvent.h"
#include "matcher/SimpleMatcher.h"
#include "scorer/MockScorer.h"
#include "core/DummyLocation.h"

using kws::core::BoundingBox;
using kws::core::Event;
using kws::core::ScoredEvent;
using kws::core::Match;
using kws::core::MatchError;
using kws::core::testing::DummyLocation;
using kws::matcher::SimpleMatcher;
using kws::scorer::testing::MockScorer;

using testing::ElementsAre;
using testing::NiceMock;
using testing::Return;

template <class E>
class MockEventSet : public kws::core::EventSet<E> {
public:
  typedef typename E::QType QType;
  typedef typename E::LType LType;
  typedef std::list<E> EventList;

  virtual ~MockEventSet() {}

  MOCK_METHOD1_T(Insert, void(const E&));
  MOCK_METHOD0(Clear, void());
  MOCK_METHOD1_T(Remove, void(const E&));
  MOCK_CONST_METHOD1_T(FindOverlapping, EventList(const Event<QType, LType>&));
};


typedef Event<int, DummyLocation> DummyEvent;

TEST(SimpleMatcherTest, AllEmpty) {
  MockScorer<DummyEvent, DummyEvent> scorer;
  SimpleMatcher<DummyEvent, DummyEvent> matcher(&scorer);
  const auto result = matcher.Match({}, {});
  EXPECT_THAT(result, testing::IsEmpty());
}

TEST(SimpleMatcherTest, RefsEmpty) {
  MockScorer<DummyEvent, DummyEvent> scorer;
  SimpleMatcher<DummyEvent, DummyEvent> matcher(&scorer);
  const std::vector<DummyEvent> hyps{DummyEvent(1, 1)};
  const auto result = matcher.Match({}, hyps);
  EXPECT_THAT(result, ElementsAre(
      Match<DummyEvent, DummyEvent>::MakeFalsePositive(hyps[0])));
}

TEST(SimpleMatcherTest, HypsEmpty) {
  MockScorer<DummyEvent, DummyEvent> scorer;
  SimpleMatcher<DummyEvent, DummyEvent> matcher(&scorer);
  const std::vector<DummyEvent> refs{DummyEvent(1, 1)};
  const auto result = matcher.Match(refs, {});
  EXPECT_THAT(result, ElementsAre(
      Match<DummyEvent, DummyEvent>::MakeFalseNegative(refs[0])));
}

TEST(SimpleMatcherTest, NoOverlaps) {
  MockScorer<DummyEvent, DummyEvent> scorer;
  SimpleMatcher<DummyEvent, DummyEvent> matcher(&scorer);
  const std::vector<DummyEvent> refs{DummyEvent(1, 1)};
  const std::vector<DummyEvent> hyps{DummyEvent(1, 2)};
  const auto result = matcher.Match(refs, hyps);
  EXPECT_THAT(result, ElementsAre(
      Match<DummyEvent, DummyEvent>::MakeFalsePositive(hyps[0]),
      Match<DummyEvent, DummyEvent>::MakeFalseNegative(refs[0])));
}

TEST(SimpleMatcherTest, PerfectMatch) {
  MockScorer<DummyEvent, DummyEvent> scorer;
  NiceMock<MockEventSet<DummyEvent>>* refs_set = new NiceMock<MockEventSet<DummyEvent>>();
  SimpleMatcher<DummyEvent, DummyEvent> matcher(&scorer, refs_set);

  const std::vector<DummyEvent> refs{DummyEvent(1, 1)};
  const std::vector<DummyEvent> hyps{DummyEvent(1, 1)};

  EXPECT_CALL(*refs_set, FindOverlapping(hyps[0]))
      .WillOnce(Return(std::list<DummyEvent>{refs[0]}));
  EXPECT_CALL(scorer, ComputeError(refs[0], hyps[0]))
      .WillOnce(Return(MatchError(0, 0)));

  const auto result = matcher.Match(refs, hyps);
  EXPECT_THAT(result, ElementsAre(
      Match<DummyEvent, DummyEvent>(refs[0], hyps[0], MatchError(0, 0))));
}

TEST(SimpleMatcherTest, NotPerfectMatches) {
  MockScorer<DummyEvent, DummyEvent> scorer;
  NiceMock<MockEventSet<DummyEvent>>* refs_set = new NiceMock<MockEventSet<DummyEvent>>();
  SimpleMatcher<DummyEvent, DummyEvent> matcher(&scorer, refs_set);

  {
    const std::vector<DummyEvent> refs{DummyEvent(1, 1), DummyEvent(1, 2)};
    const std::vector<DummyEvent> hyps{DummyEvent(1, 1)};

    // Suppose that the location overlaps with the two reference events.
    EXPECT_CALL(*refs_set, FindOverlapping(hyps[0]))
        .WillOnce(Return(std::list<DummyEvent>{refs[0], refs[1]}));
    // The scorer gives a null true positive ratio for the first event.
    EXPECT_CALL(scorer, ComputeError(refs[0], hyps[0]))
        .WillOnce(Return(MatchError(1, 1)));
    // And a non-zero positive ratio for the second event.
    EXPECT_CALL(scorer, ComputeError(refs[1], hyps[0]))
        .WillOnce(Return(MatchError(0.5, 0.3)));

    const auto result = matcher.Match(refs, hyps);
    EXPECT_THAT(result, ElementsAre(
        Match<DummyEvent, DummyEvent>(refs[1], hyps[0], MatchError(0.5, 0.3)),
        Match<DummyEvent, DummyEvent>::MakeFalseNegative(refs[0])));
  }

  {
    // Hypothesis matches two reference events, only one should be considered
    // (the one with largest overlap).
    const std::vector<DummyEvent> refs{DummyEvent(1, 1), DummyEvent(1, 2)};
    const std::vector<DummyEvent> hyps{DummyEvent(1, 1)};

    // Suppose that the location overlaps with the two reference events.
    // The intersection area with the second reference is > than with
    // the first reference.
    EXPECT_CALL(*refs_set, FindOverlapping(hyps[0]))
        .WillOnce(Return(std::list<DummyEvent>{refs[1], refs[0]}));
    // Since we match against refs[1], refs[0] will not be considered
    EXPECT_CALL(scorer, ComputeError(refs[1], hyps[0]))
        .WillOnce(Return(MatchError(0, 0.2)));

    const auto result = matcher.Match(refs, hyps);
    EXPECT_THAT(result, ElementsAre(
        Match<DummyEvent, DummyEvent>(refs[1], hyps[0], MatchError(0, 0.2)),
        Match<DummyEvent, DummyEvent>::MakeFalseNegative(refs[0])));
  }

  {
    // Reference matches two hypotheses. Only one should be considered, the second
    // one should be ignored (even if the tp ratio is greater).
    const std::vector<DummyEvent> refs{DummyEvent(1, 1)};
    const std::vector<DummyEvent> hyps{DummyEvent(1, 1), DummyEvent(1, 2)};

    EXPECT_CALL(*refs_set, FindOverlapping(hyps[0]))
        .WillOnce(Return(std::list<DummyEvent>{refs[0]}));
    EXPECT_CALL(*refs_set, FindOverlapping(hyps[1]))
        .WillOnce(Return(std::list<DummyEvent>{refs[0]}));

    // Notice that the match against hyp2 would be better, however since the
    // reference was already matched we won't increase precision or recall.
    EXPECT_CALL(scorer, ComputeError(refs[0], hyps[0]))
        .WillOnce(Return(MatchError(0.4, 0.5)));
    EXPECT_CALL(scorer, ComputeError(refs[0], hyps[1]))
        .WillOnce(Return(MatchError(0.0, 0.2)));

    // Notice that there are not false positives, nor false negatives!
    const auto result = matcher.Match(refs, hyps);
    EXPECT_THAT(result, ElementsAre(
        Match<DummyEvent, DummyEvent>(refs[0], hyps[0], MatchError(0.4, 0.5))));
  }
}
