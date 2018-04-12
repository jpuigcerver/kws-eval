#ifndef CORE_BOOTSTRAPPING_H_
#define CORE_BOOTSTRAPPING_H_

#include <algorithm>
#include <random>
#include <vector>

#include "core/Match.h"

namespace kws {
namespace core {

template<typename RefEvent, typename HypEvent>
class MatchesSampler {
 public:
  typedef std::vector<Match<RefEvent, HypEvent>> Container;

  explicit MatchesSampler(const size_t random_seed)
      : rng_(random_seed) {}

  void operator()(const Container &original, Container *sampled) {
    const size_t num_matches = original.size();
    std::uniform_int_distribution <size_t> mdist(0, num_matches - 1);
    sampled->resize(num_matches);
    for (size_t i = 0; i < num_matches; ++i) {
      sampled->push_back(original[mdist(rng_)]);
    }
  }

 private:
  std::default_random_engine rng_;
};

template<typename RefEvent, typename HypEvent>
class MatchesByQuerySampler {
 public:
  typedef std::vector<std::vector<Match<RefEvent, HypEvent>>> Container;

  explicit MatchesByQuerySampler(const size_t random_seed)
      : rng_(random_seed) {}

  void operator()(const Container &original, Container *sampled) {
    const size_t num_queries = original.size();
    std::uniform_int_distribution <size_t> qdist(0, num_queries - 1);
    sampled->clear();
    sampled->resize(num_queries);
    for (size_t i = 0; i < num_queries; ++i) {
      const size_t q = qdist(rng_);
      const size_t num_matches_q = original[q].size();
      std::uniform_int_distribution <size_t> mdist(0, num_matches_q - 1);
      for (size_t j = 0; j < num_matches_q; ++j) {
        const auto &m = original[q][mdist(rng_)];
        (*sampled)[i].push_back(m);
      }
    }
  }

 private:
  std::default_random_engine rng_;
};

template <typename Container, typename Statistic, typename Sampler>
double ComputePercentileBootstrapCI(
    const Container& original_samples, size_t repetitions, double alpha,
    Statistic statistic, Sampler *sampler,
    double *lower_bound, double *upper_bound) {
  // Compute observed statistic
  const double observed_statistic = statistic(original_samples);
  std::vector<double> statistics_diffs;
  for (size_t r = 0; r < repetitions; ++r) {
    // Build bootstrapped sample
    Container bootstrapped_sample;
    (*sampler)(original_samples, &bootstrapped_sample);
    // Compute the statistic for the bootstrapped sample
    const double sample_statistic = statistic(bootstrapped_sample);
    // Store the difference w.r.t. the observed statistic
    statistics_diffs.push_back(sample_statistic - observed_statistic);
  }
  std::sort(statistics_diffs.begin(), statistics_diffs.end());
  const auto li = (size_t)((1.0 - alpha * 0.5) * repetitions);
  const auto ui = (size_t)((      alpha * 0.5) * repetitions);
  *lower_bound = observed_statistic - statistics_diffs[li];
  *upper_bound = observed_statistic - statistics_diffs[ui];
  return observed_statistic;
}

}  // namespace core
}  // namespace kws

#endif  // CORE_BOOTSTRAPPING_H_
