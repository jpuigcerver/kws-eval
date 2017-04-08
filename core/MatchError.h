#ifndef CORE_MATCHERROR_H_
#define CORE_MATCHERROR_H_

#include <iostream>

namespace kws {
namespace core {

class MatchError {
public:
  MatchError() : fp_(0.0f), fn_(0.0f) {}

  MatchError(float fp, float fn) : fp_(fp), fn_(fn) {}

  virtual ~MatchError() {}

  inline bool operator==(const MatchError& other) const {
    return fp_ == other.fp_ && fn_ == other.fn_;
  }

  inline bool operator!=(const MatchError& other) const {
    return fp_ != other.fp_ || fn_ != other.fn_;
  }

  inline float FP() const { return fp_; }

  inline float FN() const { return fn_; }

  virtual size_t NH() const { return (fp_ > 0.0f || fn_ < 1.0f) ? 1 : 0; }

  virtual size_t NR() const { return (fp_ < 1.0f || fn_ > 0.0f) ? 1 : 0; }

protected:
  float fp_, fn_;
};

std::ostream& operator<<(std::ostream& oss, const MatchError& errors) {
  oss << "MatchError[fp=" << errors.FP() << ", fn=" << errors.FN()
      << ", nh=" << errors.NH() << ", nr=" << errors.NR() << "]";
  return oss;
}

}  // namespace core
}  // namespace kws

#endif  // CORE_MATCHERROR_H_
