#ifndef SCORER_SOFTINTERSECTIONOVERHYPOTHESISAREA_H_
#define SCORER_SOFTINTERSECTIONOVERHYPOTHESISAREA_H_

#include "scorer/Scorer.h"

namespace kws {
namespace scorer {

using kws::core::MatchError;

template <class RE, class HE>
class SoftIntersectionOverHypothesisAreaScorer : public Scorer<RE, HE> {
 public:
  SoftIntersectionOverHypothesisAreaScorer() {}

  ~SoftIntersectionOverHypothesisAreaScorer() override {}

  MatchError operator()(const RE& ref, const HE& hyp) override {
    if (ref.Query() != hyp.Query()) { return MatchError{1.0f, 1.0f}; }
    const float intersect_area = IntersectionArea(ref, hyp);
    const float fp =
        1.0f - (hyp.Area() > 0.0f ? intersect_area / hyp.Area() : 0.0f);
    const float fn =
        1.0f - (ref.Area() > 0.0f ? intersect_area / ref.Area() : 0.0f);
    return MatchError{fp, fn};
  }
};

}  // namespace scorer
}  // namespace kws

#endif  // SCORER_SOFTINTERSECTIONOVERHYPOTHESISAREA_H_
