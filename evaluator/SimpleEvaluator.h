#ifndef EVALUATOR_SIMPLEEVALUATOR_H_
#define EVALUATOR_SIMPLEEVALUATOR_H_

#include <algorithm>
#include <set>

#include "evaluator/Evaluator.h"
#include "evaluator/PairedEvent.h"
#include "core/EventSet.h"

namespace kws {
namespace evaluator {

using kws::core::EventSet;

template <class RE, class HE, class OverlapScorer>
class SimpleEvaluator : public Evaluator<RE, HE> {
 public:
  typedef typename OverlapScorer::Score OverlapScore;

  explicit SimpleEvaluator(const OverlapScorer& scorer) :
      scorer_(new OverlapScorer(scorer)), owner_(true) {}

  explicit SimpleEvaluator(OverlapScorer&& scorer) :
      scorer_(&scorer), owner_(true) {}

  SimpleEvaluator(OverlapScorer* scorer, bool owner = true) :
      scorer_(scorer), owner_(true) {}

  virtual ~SimpleEvaluator() {
    if (owner_) delete scorer_;
  }

  virtual std::vector<PairedEvent<RE, HE>> Evaluate(
      const std::vector<RE>& refs, const std::vector<HE>& hyps) override {
    EventSet<RE> refs_set(refs.begin(), refs.end());
    std::set<RE> unmatched_refs(refs.begin(), refs.end());
    std::set< std::pair<RE, std::string> > matched_pairs;

    // If the given hypotheses were not sorted in decreasing orther,
    // copy them and sort them.
    const std::vector<HE>* hyps_aux = &hyps;
    if (!std::is_sorted(hyps.begin(), hyps.end(), std::greater<HE>())) {
      hyps_aux = new std::vector<HE>(hyps.begin(), hyps.end());
      auto nc_hyps = const_cast<std::vector<HE>*>(hyps_aux);
      std::sort(nc_hyps->begin(), nc_hyps->end(), std::greater<HE>());
    }

    std::vector<PairedEvent<RE, HE>> result_;

    for (const HE& hyp : *hyps_aux) {
      const RE* ref = GetBestOverlapReference(hyp, refs_set);
      if (ref != nullptr) {
        if (matched_pairs.find(make_pair(*ref, hyp.query)) !=
            matched_pairs.end()) {
          // This reference was already matched with this query. We do not
          // penalize the additional event, but we should not artifically
          // increase precision/recall either.
          result_.push_back(PairedEvent<RE, HE>(1.0, 0.0, 0.0, ref, &hyp));
          matched_pairs.insert(make_pair(*ref, hyp.query));
          unmatched_refs.erase(*ref);
        } else {
          // Do, nothing. Notice that this means that this detection is
          // ignored.
        }
      } else {
        // This is a false positive!
        result_.push_back(PairedEvent<RE, HE>(0.0, 1.0, 0.0, nullptr, &hyp));
      }
    }

    for (const RE& ref : unmatched_refs) {
      result_.push_back(PairedEvent<RE, HE>(0.0, 0.0, 1.0, &ref, nullptr));
    }

    // If we made a copy of the hypotheses, delete them.
    if (hyps_aux != &hyps) {
      delete hyps_aux;
    }

    return result_;
  }

 private:
  const RE* GetBestOverlapReference(const HE& hyp, const EventSet<RE>& refs) {
    std::vector<std::pair<OverlapScore, const RE*>> scored_overlaps;
    for (const RE& ref : refs.FindOverlapping(hyp)) {
      OverlapScore score = scorer_->operator()(ref, hyp);
      if (score.IsValid()) {
        scored_overlaps.push_back(std::make_pair(score, &ref));
      }
    }
    std::sort(scored_overlaps.begin(), scored_overlaps.end(),
              std::greater<std::pair<OverlapScore, const RE*>>());
    if (scored_overlaps.empty()) return nullptr;
    else return scored_overlaps.front().second;
  }

  OverlapScorer* scorer_;
  bool owner_;
};

}  // namespace evaluator
}  // namespace kws

#endif  // EVALUATOR_SIMPLEEVALUATOR_H_
