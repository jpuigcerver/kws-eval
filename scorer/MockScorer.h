#ifndef SCORER_MOCKSCORER_H_
#define SCORER_MOCKSCORER_H_

#include "scorer/Scorer.h"

namespace kws {
namespace scorer {
namespace testing {

template <class RE, class HE>
class MockScorer : public Scorer<RE, HE> {
 public:
  virtual ~MockScorer() {}

  MOCK_METHOD2_T(ComputeError, MatchError(const RE&, const HE&));

  MatchError operator()(const RE& ref, const HE& hyp) override {
    return ComputeError(ref, hyp);
  }
};

}  // testing
}  // namespace scorer
}  // namespace kws


#endif  // SCORER_MOCKSCORER_H_
