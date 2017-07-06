#ifndef MATCHER_SIMPLEMATCHER_H_
#define MATCHER_SIMPLEMATCHER_H_

#include <algorithm>
#include <set>
#include <vector>

#include "core/EventSet.h"
#include "core/Match.h"
#include "core/MatchError.h"
#include "matcher/Matcher.h"
#include "scorer/Scorer.h"

namespace kws {
namespace matcher {

using kws::core::EventSet;
using kws::core::MatchError;
using kws::scorer::Scorer;

template <class RE, class HE>
class SimpleMatcher : public Matcher<RE, HE> {
 public:
  typedef RE RefEvent;
  typedef HE HypEvent;
  typedef typename Matcher<RE, HE>::Result Result;

  explicit SimpleMatcher(Scorer<RefEvent, HypEvent>* scorer) :
      scorer_(scorer), refs_set_(new EventSet<RE>()) {}

  SimpleMatcher(Scorer<RefEvent, HypEvent>* scorer, EventSet<RE>* refs_set)
      : scorer_(scorer), refs_set_(refs_set) {}

  Result Match(const std::vector<RE>& refs, const std::vector<HE>& hyps)
      override {
    // Add references to the EventSet, for fast overlapping calculations.
    refs_set_->Clear();
    for (const RE& ref : refs) refs_set_->Insert(ref);
    // Add all references to the unmatched references set.
    std::set<RE> unmatched_refs(refs.begin(), refs.end());
    // Store matched hypothesis with already matched reference here.
    repeated_matches_.clear();
    Result result;
    for (const HE& hyp : hyps) {
      const auto overlapping_refs = refs_set_->FindOverlapping(hyp);
      bool matched_hyp = false;
      for (const RE &ref : overlapping_refs) {
        // Score the match
        const auto errors = scorer_->operator()(ref, hyp);
        if (errors.FP() < 1.0f) {
          // If the hypothesis matches (tp > 0) ANY reference, we will NOT
          // penalize either precision or recall, even if the reference was
          // matched before...
          matched_hyp = true;
          const auto m = kws::core::Match<RE,HE>(ref, hyp, errors);
          // ... although, we don't penalize multiple matches against the
          // same reference, we MUST NOT increase the precision/recall either.
          if (unmatched_refs.find(ref) != unmatched_refs.end()) {
            unmatched_refs.erase(ref);
            result.push_back(m);
            // This hypothesis cannot be matched again.
            break;
          } else {
            // Keep the match, just for debugging purposes.
            repeated_matches_.push_back(m);
          }
        }
      }
      // This hyp is a false positive, since it was not matched against any
      // reference.
      if (!matched_hyp) {
        result.push_back(kws::core::Match<RE,HE>::MakeFalsePositive(hyp));
      }
    }

    // Process false negatives, i.e. reference objects that were not matched
    // with any hypothesis.
    for (const RE& ref : unmatched_refs) {
      result.push_back(kws::core::Match<RE,HE>::MakeFalseNegative(ref));
    }

    return result;
  }

  const Result& GetRepeatedMatches() const {
    return repeated_matches_;
  }

 private:
  Scorer<RE, HE>* scorer_;
  std::unique_ptr<EventSet<RE>> refs_set_;
  Result repeated_matches_;
};

}  // namespace matcher
}  // namespace kws

#endif  // MATCHER_SIMPLEMATCHER_H_
