#ifndef EVALUATOR_EVALUATOR_H_
#define EVALUATOR_EVALUATOR_H_

#include <vector>

#include "evaluator/PairedEvent.h"

namespace kws {
namespace evaluator {

template <class RE, class HE>
class Evaluator {
 public:
  virtual ~Evaluator() {}

  virtual std::vector<PairedEvent<RE, HE>> Evaluate(
      const std::vector<RE>& refs, const std::vector<HE>& hyps) = 0;
};

}  // namespace evaluator
}  // namespace kws

#endif  // EVALUATOR_EVALUATOR_H_
