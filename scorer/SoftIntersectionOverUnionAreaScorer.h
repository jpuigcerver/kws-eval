#ifndef SCORER_SOFTINTERSECTIONOVERUNIONAREA_H_
#define SCORER_SOFTINTERSECTIONOVERUNIONAREA_H_

#include "scorer/Scorer.h"

namespace kws {
namespace scorer {

template <class RE, class HE>
class SoftIntersectionOverUnionAreaScorer : public Scorer<RE, HE> {
 public:
  typedef RE RefEvent;
  typedef HE HypEvent;
  typedef typename Scorer<RE, HE>::Result Result;

  SoftIntersectionOverUnionAreaScorer() {}

  ~SoftIntersectionOverUnionAreaScorer() override {}

  Result operator()(const RefEvent& ref, const HypEvent& hyp) override {
    const float union_area = UnionArea(ref, hyp);
    const float intersect_area = IntersectionArea(ref, hyp);
    const float tp = union_area > 0.0f ? intersect_area / union_area : 0.0f;
    const float fp = 1.0f - (hyp.Area() > 0 ?
                             intersect_area / hyp.Area() : 0.0f);
    const float fn = 1.0f - (ref.Area() > 0 ?
                             intersect_area / ref.Area() : 0.0f);
    return Result{tp, fp, fn};
  }
};

}  // namespace scorer
}  // namespace kws

#endif  // SCORER_SOFTINTERSECTIONOVERUNIONAREA_H_
