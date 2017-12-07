#ifndef SCORER_INTERSECTIONOVERUNIONAREASCORER_H_
#define SCORER_INTERSECTIONOVERUNIONAREASCORER_H_

#ifdef WITH_GLOG
#include <glog/logging.h>
#endif

#include "scorer/Scorer.h"

namespace kws {
namespace scorer {

using kws::core::MatchError;

template <class RE, class HE>
class IntersectionOverUnionAreaScorer : public Scorer<RE, HE> {
 public:
  explicit IntersectionOverUnionAreaScorer(float threshold) :
      threshold_(threshold) {}

  ~IntersectionOverUnionAreaScorer() override {}

  MatchError operator()(const RE& ref, const HE& hyp) override {
    if (ref.Query() == hyp.Query() && hyp.Area() > 0) {
      const float val = IntersectionArea(ref, hyp) / (1.0f * UnionArea(ref, hyp));
#if defined(WITH_GLOG) && defined(WITH_GLOG_TRACE)
      DLOG(INFO) << "Evaluating match:" << std::endl
                 << "Ref = " << ref << std::endl
                 << "Hyp = " << hyp << std::endl
                 << "Score =" << val;
#endif
      if (val >= threshold_) return MatchError{0.0f, 0.0f};
    }
    return MatchError{1.0f, 1.0f};
  }

  inline const float& Threshold() const { return threshold_; }

 private:
  const float threshold_;
};

}  // namespace scorer
}  // namespace kws

#endif  // SCORER_INTERSECTIONOVERUNIONAREASCORER_H_
