#ifndef SCORER_TRIVIALSCORER_H
#define SCORER_TRIVIALSCORER_H

#include "scorer/Scorer.h"

namespace kws {
namespace scorer {

// This scorer just checks whether or not the Location & Query of the events
// matches or not.
template <class RE, class HE>
class TrivialScorer : public Scorer<RE, HE> {
 public:
   TrivialScorer() {}

   ~TrivialScorer() override {}

   MatchError operator()(const RE& ref, const HE& hyp) override {
     if (ref.Location() == hyp.Location() && ref.Query() == hyp.Query()) {
       return MatchError(0, 0);
     } else {
       return MatchError(1, 1);
     }
   }
};

}  // namespace scorer
}  // namespace kws

#endif // SCORER_TRIVIALSCORER_H
