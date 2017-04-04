#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/DocumentBoundingBox.h"
#include "core/ScoredEvent.h"
#include "scorer/FloatScore.h"
#include "evaluator/SimpleEvaluator.h"

using kws::core::DocumentBoundingBox;
using kws::core::Event;
using kws::core::ScoredEvent;
using kws::evaluator::SimpleEvaluator;

typedef Event<DocumentBoundingBox<int>> RefEvent;
typedef ScoredEvent<DocumentBoundingBox<int>> HypEvent;

namespace kws {
namespace score {
namespace testing {

class MockScorer {
 public:
  typedef kws::scorer::FloatScore Score;
  MockScorer() {}
  virtual ~MockScorer() {}

  MOCK_METHOD2(ComputeScore, Score(const RefEvent&, const HypEvent&));

  virtual Score operator()(const RefEvent& ref, const HypEvent& hyp) {
    return ComputeScore(ref, hyp);
  }
};

}  // namespace testing
}  // namespace score
}  // namespace kws

TEST(SimpleEvaluator, Constructor) {
  using kws::score::testing::MockScorer;
  MockScorer scorer;
  SimpleEvaluator<RefEvent, HypEvent, MockScorer> evaluator(&scorer, false);

}
