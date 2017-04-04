#ifndef SCORER_INTERSECTIONOVERUNIONAREA_H_
#define SCORER_INTERSECTIONOVERUNIONAREA_H_

#include "scorer/Scorer.h"

namespace kws {
namespace scorer {

template <class RE, class HE>
class IntersectionOverUnionAreaScorer : public Scorer<RE, HE> {
 public:
  typedef RE RefEvent;
  typedef HE HypEvent;
  typedef typename Scorer<RE, HE>::Result Result;

  IntersectionOverUnionAreaScorer(float threshold) :
      threshold_(threshold) {}

  ~IntersectionOverUnionAreaScorer() override {}

  Result operator()(const RefEvent& ref, const HypEvent& hyp) override {
    const float union_area = UnionArea(ref, hyp);
    if (union_area > 0) {
      const float val = IntersectionArea(ref, hyp) / union_area;
      if (val >= threshold_) return Result{1.0f, 0.0f, 0.0f};
    }
    return Result{0.0f, 1.0f, 1.0f};
  }

  inline const float& Threshold() const { return threshold_; }

 private:
  const float threshold_;
};

}  // namespace scorer
}  // namespace kws

#endif  // SCORER_INTERSECTIONOVERUNIONAREA_H_
