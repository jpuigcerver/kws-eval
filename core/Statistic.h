#ifndef CORE_STATISTIC_H_
#define CORE_STATISTIC_H_

#include <vector>

#include "core/Assessment.h"

namespace kws {
namespace core {

template<typename Match>
class Statistic {
 public:
  explicit Statistic(bool collapse_matches)
      : collapse_matches_(collapse_matches) {}

  virtual ~Statistic() {}

  virtual double operator()(
      const std::vector<std::vector<Match>> &grouped_matches,
      bool sort_matches) const = 0;

 protected:
  bool collapse_matches_;
};

template<typename Match>
class GlobalStatistic : public Statistic<Match> {
 public:
  using Statistic<Match>::operator();

  explicit GlobalStatistic(bool collapse_matches)
      : Statistic<Match>(collapse_matches) {}

  virtual double operator()(const std::vector<Match> &matches) const = 0;
};

template<typename Match>
class GlobalAP : public GlobalStatistic<Match> {
 public:
  explicit GlobalAP(bool collapse_matches, bool interpolate_precision,
                    bool trapezoid_integral)
      : GlobalStatistic<Match>(collapse_matches),
        interpolate_precision_(interpolate_precision),
        trapezoid_integral_(trapezoid_integral) {}

  double operator()(const std::vector<Match> &matches) const override {
    return ComputeGlobalAP<Match>(
        matches, GlobalStatistic<Match>::collapse_matches_,
        interpolate_precision_, trapezoid_integral_);
  }

  double operator()(
      const std::vector<std::vector<Match>> &grouped_matches,
      bool sort_matches) const override {
    return ComputeGlobalAP<Match>(
        grouped_matches, GlobalStatistic<Match>::collapse_matches_,
        interpolate_precision_, trapezoid_integral_, sort_matches);
  }

 private:
  bool interpolate_precision_, trapezoid_integral_;
};

template<typename Match>
class MeanAP : public Statistic<Match> {
 public:
  explicit MeanAP(bool collapse_matches, bool interpolate_precision,
                  bool trapezoid_integral)
      : Statistic<Match>(collapse_matches),
        interpolate_precision_(interpolate_precision),
        trapezoid_integral_(trapezoid_integral) {}

  double operator()(
      const std::vector<std::vector<Match>> &grouped_matches,
      bool sort_matches) const override {
    return ComputeMeanAP(grouped_matches, Statistic<Match>::collapse_matches_,
                         interpolate_precision_, trapezoid_integral_,
                         sort_matches);
  }

 private:
  bool interpolate_precision_, trapezoid_integral_;
};

template<typename Match>
class GlobalNDCG : public GlobalStatistic<Match> {
 public:
  explicit GlobalNDCG(bool collapse_matches)
      : GlobalStatistic<Match>(collapse_matches) {}

  double operator()(const std::vector<Match> &matches) const override {
    return ComputeGlobalNDCG(matches, GlobalStatistic<Match>::collapse_matches_);
  }

  double operator()(
      const std::vector<std::vector<Match>> &grouped_matches,
      bool sort_matches) const override {
    return ComputeGlobalNDCG(grouped_matches,
                             GlobalStatistic<Match>::collapse_matches_,
                             sort_matches);
  }
};

template<typename Match>
class MeanNDCG : public Statistic<Match> {
 public:
  explicit MeanNDCG(bool collapse_matches)
      : Statistic<Match>(collapse_matches) {}

  double operator()(
      const std::vector<std::vector<Match>> &grouped_matches,
      bool sort_matches) const override {
    return ComputeMeanNDCG(grouped_matches, Statistic<Match>::collapse_matches_,
                           sort_matches);
  }
};

}  // namespace core
}  // namespace kws

#endif  // CORE_BOOTSTRAPPING_H_