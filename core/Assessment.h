#ifndef CORE_ASSESSMENT_H
#define CORE_ASSESSMENT_H

#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/MatchError.h"
#include "core/MatchErrorCounts.h"

namespace kws {
namespace core {

template<typename M, typename Map>
void GroupMatchesByQueryGroup(
    const std::vector<M> &matches, const Map &query_to_group,
    std::vector<std::vector<M>> *matches_by_group) {
  typedef typename M::RefEvent::QType QType;
  typedef typename Map::size_type SType;
  matches_by_group->clear();
  std::unordered_map<QType, SType> group2pos;
  for (const auto &m : matches) {
    const auto &query = m.HasRef() ? m.GetRef().Query() : m.GetHyp().Query();
    auto it = query_to_group.find(query);
    const auto &group = it != query_to_group.end() ? it->second : query;
    const size_t pos = group2pos.emplace(group, group2pos.size()).first->second;
    if (pos < matches_by_group->size()) {
      (*matches_by_group)[pos].push_back(m);
    } else {
      matches_by_group->emplace_back();
      matches_by_group->back().push_back(m);
    }
  }
}

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
template<typename Container>
std::vector<MatchErrorCounts> CollapseMatches(const Container &matches) {
  std::vector<std::pair<MatchErrorCounts, float>> collapsed_matches;
  for (const auto &m : matches) {
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
  for (const auto &cm : collapsed_matches) {
    output.push_back(cm.first);
  }
  return output;
}

template<typename Container>
std::vector<MatchError> GetMatchErrors(const Container &matches) {
  std::vector<MatchError> output;
  for (const auto &m : matches) {
    output.push_back(m.GetError());
  }
  return output;
}

template<typename Container>
void SortMatchesDecreasingScore(Container *matches) {
  typedef typename Container::value_type Match;
  std::sort(matches->begin(), matches->end(),
            [](const Match &a, const Match &b) -> bool {
              if (a.HasHyp() && b.HasHyp()) {
                return a.GetHyp().Score() > b.GetHyp().Score();
              } else {
                return a.HasHyp();
              }
            });
}

// Compute Precision and Recall points
template<typename Real, class Container>
void ComputePrecisionAndRecall(
    const Container &errors, bool interpolate,
    std::vector<Real> *pr, std::vector<Real> *rc) {
  typedef typename Container::value_type ME;
  static_assert(std::is_base_of<MatchError, ME>::value,
                "Errors must be descendant of MatchError class.");
  pr->clear();
  rc->clear();

  // Count total number of references
  const auto TR =
      std::accumulate(errors.begin(), errors.end(), 0,
                      [](size_t a, const ME &e) -> size_t {
                        return a + e.NR();
                      });

  size_t sumNR = 0, sumNH = 0;
  Real sumFP = 0, sumFN = 0;
  for (const auto &e : errors) {
    sumNR += e.NR();  // total number of matched references so far
    sumNH += e.NH();  // total number of matched hypotheses so far
    sumFP += e.FP();  // total number (fractional) of false positives
    sumFN += e.FN();  // total number (fractional) of false negatives
    // Precision at this point:
    // Ill-posed case: sumNH = 0, p = 1.0.
    // General case:   sumNH > 0, p = 1.0 - (sumFP / sumNH)
    const Real p = sumNH > 0 ? 1.0 - sumFP / sumNH : 1.0;
    // Recall at this point:
    // Ill-posed case: TR = 0, r = 1.0
    // General case:   TR > 0, r = (sumNR - sumFN) / TR
    const Real r = TR > 0 ? (sumNR - sumFN) / TR : 1.0;
    pr->push_back(p);
    rc->push_back(r);
  }
  if (interpolate && errors.size() > 1) {
    for (size_t i = errors.size() - 1; i > 0; --i) {
      pr->at(i - 1) = std::max(pr->at(i - 1), pr->at(i));
    }
  }
}

template<typename Real, typename Container>
Real ComputeAP(const Container &errors, const std::vector<Real> &pr,
               bool trapezoid) {
  typedef typename Container::value_type ME;
  static_assert(std::is_base_of<MatchError, ME>::value,
                "Errors must be descendant of MatchError class.");
  assert(pr.size() == errors.size());
  // Count total number of references (relevant objects)
  const auto TR =
      std::accumulate(errors.begin(), errors.end(), 0,
                      [](size_t a, const ME &e) -> size_t {
                        return a + e.NR();
                      });
  // AP = 1/TR * (\sum_{i} pr(i) * tp(i))
  Real sumAP = 0.0;
  for (size_t i = 0; i < pr.size(); ++i) {
    const Real tp = (errors[i].NH() - errors[i].FP());
    sumAP += tp * (trapezoid && i > 0 ? 0.5 * (pr[i] + pr[i - 1]) : pr[i]);
  }

  return TR > 0 ? sumAP / TR : 0.0f;
}

// Compute Average Precision from the precision and recall curves, using
// equation: AP = \sum_{i} Pr[i] * (Rc[i] - Rc[i - 1])
//              = \sum_{i} Pr[i] * Rc[i]  - \sum_{i} Pr[i] * Rc[i-1]
template<typename Real>
Real ComputeAP(const std::vector<Real> &pr, const std::vector<Real> &rc,
               bool trapezoid) {
  assert(pr.size() == rc.size());
  Real AP1 = 0.0;
  for (size_t i = 0; i < pr.size(); ++i) {
    AP1 += rc[i] * (trapezoid && i > 0 ? 0.5 * (pr[i] + pr[i - 1]) : pr[i]);
  }
  Real AP2 = 0.0f;
  for (size_t i = 1; i < pr.size(); ++i) {
    AP2 += rc[i - 1] * (trapezoid ? 0.5 * (pr[i] + pr[i - 1]) : pr[i]);
  }
  return AP1 - AP2;
}

template<typename Real, typename Container>
Real ComputeNDCG(const Container &errors) {
  typedef typename Container::value_type ME;
  // Compute unnormalized DCG
  Real dcg = 0.0;
  size_t i = 1;
  for (auto it = errors.begin(); it != errors.end(); ++it, ++i) {
    const Real r = it->NH() / static_cast<Real>(it->NH() + it->NR());
    dcg += (pow(2.0, r) - 1) / log2(i);
  }
  // Count total number of references (relevant objects)
  const auto TR = std::accumulate<typename Container::const_iterator, size_t>(
      errors.begin(), errors.end(), 0,
      [](size_t a, const ME &e) -> size_t {
        return a + e.NR();
      });
  Real Z = 0.0;
  for (i = 1; i <= TR; ++i) {
    Z += 1.0 / log2(i);
  }
  return dcg / Z;
}

template<typename M>
double ComputeGlobalAP(
    const std::vector<M> &matches, bool collapse_matches,
    bool interpolate_precision, bool trapezoid_integral) {
  std::vector<double> pr, rc;
  if (collapse_matches) {
    // Collapse all events with the same score into a single point in the
    // precision/recall curve.
    const auto collapsed_errors = CollapseMatches(matches);
    // Compute precision and recall curves
    ComputePrecisionAndRecall(collapsed_errors,
                              interpolate_precision,
                              &pr, &rc);
  } else {
    const auto errors = GetMatchErrors(matches);
    ComputePrecisionAndRecall(errors,
                              interpolate_precision,
                              &pr, &rc);
  }
  return ComputeAP(pr, rc, trapezoid_integral);
}

template<typename Match>
double ComputeGlobalAP(
    const std::vector<std::vector<Match>> &matches_by_query,
    bool collapse_matches, bool interpolate_precision,
    bool trapezoid_integral, bool sort_matches) {
  // Put matches per query into a single vector.
  std::vector<Match> all_matches;
  for (const auto &matches_vector : matches_by_query) {
    for (const auto &match : matches_vector) {
      all_matches.push_back(match);
    }
  }
  if (sort_matches) { SortMatchesDecreasingScore(&all_matches); }
  // Compute precision and recall curves from the matches
  std::vector<double> pr, rc;
  if (collapse_matches) {
    const auto collapsed_errors = CollapseMatches(all_matches);
    ComputePrecisionAndRecall(collapsed_errors, interpolate_precision,
                              &pr, &rc);
  } else {
    const auto errors = GetMatchErrors(all_matches);
    ComputePrecisionAndRecall(errors, interpolate_precision, &pr, &rc);
  }
  return ComputeAP(pr, rc, trapezoid_integral);
}

template <typename Match>
double ComputeMeanAP(
    const std::vector<std::vector<Match>> &matches_by_query,
    bool collapse_matches, bool interpolate_precision,
    bool trapezoid_integral, bool sort_matches) {
  double sumAP = 0.0;
  for (auto matches_vector : matches_by_query) {
    if (sort_matches) { SortMatchesDecreasingScore(&matches_vector); }
    // Compute precision and recall curves from the matches
    std::vector<double> pr, rc;
    if (collapse_matches) {
      const auto collapsed_errors = CollapseMatches(matches_vector);
      ComputePrecisionAndRecall(collapsed_errors, interpolate_precision,
                                &pr, &rc);
    } else {
      const auto errors = GetMatchErrors(matches_vector);
      ComputePrecisionAndRecall(errors, interpolate_precision, &pr, &rc);
    }
    sumAP += ComputeAP(pr, rc, trapezoid_integral);
  }
  // Average AP across all queries.
  return matches_by_query.size() > 0 ? sumAP / matches_by_query.size() : 0.0;
}

template<typename Match>
double ComputeGlobalNDCG(
    const std::vector<Match> &matches, bool collapse_matches) {
  if (collapse_matches) {
    return ComputeNDCG<double>(CollapseMatches(matches));
  } else {
    return ComputeNDCG<double>(GetMatchErrors(matches));
  }
}

template<typename Match>
double ComputeGlobalNDCG(
    const std::vector<std::vector<Match>> &matches_by_query,
    bool collapse_matches, bool sort_matches) {
  // Put matches per query into a single vector.
  std::vector<Match> all_matches;
  for (const auto &matches_vector : matches_by_query) {
    for (const auto &match : matches_vector) {
      all_matches.push_back(match);
    }
  }
  if (sort_matches) { SortMatchesDecreasingScore(&all_matches); }
  if (collapse_matches) {
    return ComputeNDCG<double>(CollapseMatches(all_matches));
  } else {
    return ComputeNDCG<double>(GetMatchErrors(all_matches));
  }
}

template <typename Match>
double ComputeMeanNDCG(
    const std::vector<std::vector<Match>> &matches_by_query,
    bool collapse_matches, bool sort_matches) {
  double sumNDCG = 0.0;
  for (auto matches_vector : matches_by_query) {
    if (sort_matches) { SortMatchesDecreasingScore(&matches_vector); }
    if (collapse_matches) {
      sumNDCG += ComputeNDCG<double>(CollapseMatches(matches_vector));
    } else {
      sumNDCG += ComputeNDCG<double>(GetMatchErrors(matches_vector));
    }
  }
  // Average NDCG accross all queries.
  return matches_by_query.size() > 0 ? sumNDCG / matches_by_query.size() : 0.0;
}

}  // namespace core
}  // namespace kws
#endif //CORE_ASSESSMENT_H
