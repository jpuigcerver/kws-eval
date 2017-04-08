#ifndef MATCHER_MATCHER_H_
#define MATCHER_MATCHER_H_

#include <vector>

#include "core/Match.h"

namespace kws {
namespace matcher {

template <class RE, class HE>
class Matcher {
 public:
  typedef typename std::vector<kws::core::Match<RE, HE>> Result;

  virtual ~Matcher() {}

  virtual Result Match(const std::vector<RE>& refs,
                       const std::vector<HE>& hyps) = 0;
};

}  // namespace matcher
}  // namespace kws

#endif  // MATCHER_MATCHER_H_
