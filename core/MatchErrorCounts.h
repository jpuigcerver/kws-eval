#ifndef CORE_MATCHERRORCOUNTS_H_
#define CORE_MATCHERRORCOUNTS_H_

#include <iostream>
#include <tuple>

#include "core/MatchError.h"

namespace kws {
namespace core {

class MatchErrorCounts : public MatchError {
public:
  using MatchError::fp_;
  using MatchError::fn_;

  MatchErrorCounts() :
      MatchError(0.0f, 0.0f), nh_(0), nr_(0) {}

  explicit MatchErrorCounts(const MatchError& m) :
      MatchError(m), nh_(m.NH()), nr_(m.NR()) {}

  MatchErrorCounts(float fp, float fn, size_t nh, size_t nr) :
      MatchError(fp, fn), nh_(nh), nr_(nr) {}

  bool operator==(const MatchErrorCounts& other) const {
    return std::tie(fp_, fn_, nh_, nr_) ==
        std::tie(other.fp_, other.fn_, other.nh_, other.nr_);
  }

  bool operator!=(const MatchErrorCounts& other) const {
    return std::tie(fp_, fn_, nh_, nr_) !=
        std::tie(other.fp_, other.fn_, other.nh_, other.nr_);
  }

  MatchErrorCounts operator+(const MatchErrorCounts& other) const {
    return MatchErrorCounts(fp_ + other.FP(), fn_ + other.FN(),
                            nh_ + other.NH(), nr_ + other.NR());
  }

  MatchError& operator+=(const MatchError& other) {
    fp_ += other.FP();
    fn_ += other.FN();
    nh_ += other.NH();
    nr_ += other.NR();
    return *this;
  }

  size_t NH() const override { return nh_; }

  size_t NR() const override { return nr_; }

private:
  size_t nh_, nr_;
};

std::ostream& operator<<(std::ostream& oss, const MatchErrorCounts& errors) {
  oss << "MatchErrorCounts[fp=" << errors.FP() << ", fn=" << errors.FN()
      << ", nh=" << errors.NH() << ", nr=" << errors.NR() << "]";
  return oss;
}

}  // namespace core
}  // namespace kws

#endif  // CORE_MATCHERRORCOUNS_H_
