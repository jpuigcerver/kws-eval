#ifndef SCORER_SCORER_H_
#define SCORER_SCORER_H_

#include "core/MatchError.h"

namespace kws {
namespace scorer {

using kws::core::MatchError;

template <class RE, class HE>
class Scorer {
 public:
  typedef RE RefEvent;
  typedef HE HypEvent;

  Scorer() {}

  virtual ~Scorer() {}

  virtual MatchError operator()(const RefEvent& ref, const HypEvent& hyp) = 0;
};

}  // namespace scorer
}  // namespace kws

#endif  // SCORER_SCORER_H_
