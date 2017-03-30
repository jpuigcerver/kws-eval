#ifndef SCORER_INTERSECTIONOVERHYPOTHESISAREA_H_
#define SCORER_INTERSECTIONOVERHYPOTHESISAREA_H_

#include "scorer/FloatScore.h"

namespace kws {
namespace scorer {

template <class RE, class HE>
class IntersectionOverHypothesisAreaScorer {
 public:
  typedef FloatScore Score;

  IntersectionOverHypothesisAreaScorer(float threshold) :
      threshold_(threshold) {}

  Score operator()(const RE& ref, const HE& hyp) {
    if (hyp.location.Area() > 0) {
      const float val =
          ref.location.IntersectionArea(hyp.location) /
          (1.0f * hyp.location.Area());
      if (val < threshold_) return Score(); // invalid match
      else return Score(val);
    }
    return Score();  // invalid match
  }

 private:
  const float threshold_;
};

}  // namespace scorer
}  // namespace kws

#endif  // SCORER_INTERSECTIONOVERHYPOTHESISAREA_H_
