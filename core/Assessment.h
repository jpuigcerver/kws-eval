#ifndef CORE_ASSESSMENT_H
#define CORE_ASSESSMENT_H

#include <algorithm>
#include <cassert>
#include <limits>
#include <utility>
#include <vector>

#include "core/MatchError.h"
#include "core/MatchErrorCounts.h"

namespace kws {
namespace core {

// Collapse all matches with the same hypothesis score into a single point
// into the Recall-Precision curve.
// E.g:
// fp=0 fn=0 score=0.9
// fp=1 fn=0 score=0.9
// fp=0 fn=1 score=-inf
// fp=0 fn=1 score=-inf
// -----
// fp=1 fn=0 nh=2 nr=1 score=0.9
// fp=0 fn=2 nh=0 nr=2 score=-inf
template <typename Container>
std::vector<MatchErrorCounts> CollapseMatches(Container matches) {
  std::vector<std::pair<MatchErrorCounts, float>> collapsed_matches;
  for(const auto& m : matches) {
    const float score = m.HasHyp()
        ? m.GetHyp().Score()                         // score of the hypothesis
        : -std::numeric_limits<float>::infinity();  // false negative

    if (collapsed_matches.size() == 0 ||
        score != collapsed_matches.back().second) {
      // hypothesis with new score, add a new pair to the collapsed vector
      collapsed_matches.emplace_back(
          std::make_pair(MatchErrorCounts(m.GetError()), score));
    } else {
      // hypothesis with a repeated score, increment the tp, fp and fn rate.
      collapsed_matches.back().first += m.GetError();
    }
  }
  std::vector<MatchErrorCounts> output;
  for (const auto& cm : collapsed_matches) {
    output.push_back(cm.first);
  }
  return output;
}

// Compute Precision and Recall points
template <class Container>
void ComputePrecisionAndRecall(
    const Container& errors, bool interpolate,
    std::vector<float>* pr, std::vector<float>* rc) {
  typedef typename Container::value_type ME;
  static_assert(std::is_base_of<MatchError, ME>::value,
                "Errors must be descendant of MatchError class.");
  pr->clear();
  rc->clear();

  // Count total number of references
  const size_t TR =
      std::accumulate(errors.begin(), errors.end(), 0,
                      [](size_t a, const ME& e) -> size_t {
                        return a + e.NR();
                      });

  size_t sumNR = 0, sumNH = 0;
  float sumFP = 0.0f, sumFN = 0.0f;
  for (const auto& e : errors) {
    sumNR += e.NR();  // total number of matched references so far
    sumNH += e.NH();  // total number of matched hypotheses so far
    sumFP += e.FP();  // total number (fractional) of false positives
    sumFN += e.FN();  // total number (fractional) of false negatives
    // Precision at this point:
    // Ill-posed case: sumNH = 0, p = 1.0.
    // General case:   sumNH > 0, p = 1.0 - (sumFP / sumNH)
    const float p = sumNH > 0 ? 1.0f - sumFP / sumNH : 1.0f;
    // Recall at this point:
    // Ill-posed case: TR = 0, r = 1.0
    // General case:   TR > 0, r = (sumNR - sumFN) / TR
    const float r = TR > 0 ? (sumNR - sumFN) / TR : 1.0f;
    pr->push_back(p);
    rc->push_back(r);
  }
  if (interpolate && errors.size() > 1) {
    for (size_t i = errors.size() - 1; i > 0; --i) {
      pr->at(i - 1) = std::max(pr->at(i - 1), pr->at(i));
    }
  }
}

template <typename Container>
float ComputeAP(const Container& errors, const std::vector<float>& pr,
                 bool trapezoid) {
  typedef typename Container::value_type ME;
  static_assert(std::is_base_of<MatchError, ME>::value,
                "Errors must be descendant of MatchError class.");
  assert(pr.size() == errors.size());
  // Count total number of references
  const size_t TR =
      std::accumulate(errors.begin(), errors.end(), 0,
                      [](size_t a, const ME& e) -> size_t {
                        return a + e.NR();
                      });
  // AP = 1/TR * (\sum_{i} pr(i) * tp(i))
  float sumAP = 0.0;
  for (size_t i = 0; i < pr.size(); ++i) {
    const float tp = (errors[i].NH() - errors[i].FP());
    sumAP += tp * (trapezoid && i > 0 ? 0.5 * (pr[i] + pr[i - 1]) : pr[i]);
  }

  return TR > 0 ? sumAP / TR : 0.0;
}

// Compute Average Precision from the precision and recall curves, using
// equation: AP = \sum_{i} Pr[i] * (Rc[i] - Rc[i - 1])
//              = \sum_{i} Pr[i] * Rc[i]  - \sum_{i} Pr[i] * Rc[i-1]
float ComputeAP(const std::vector<float>& pr, const std::vector<float>& rc,
                 bool trapezoid) {
  assert(pr.size() == rc.size());
  float AP1 = 0.0;
  for (size_t i = 0; i < pr.size(); ++i) {
    AP1 += rc[i] * (trapezoid && i > 0 ? 0.5 * (pr[i] + pr[i - 1]) : pr[i]);
  }
  float AP2 = 0.0f;
  for (size_t i = 1; i < pr.size(); ++i) {
    AP2 += rc[i - 1] * (trapezoid ? 0.5 * (pr[i] + pr[i-1]) : pr[i]);
  }
  return AP1 - AP2;
}

}
}
#endif //CORE_ASSESSMENT_H
