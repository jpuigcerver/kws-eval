#ifndef SCORER_FLOATSCORE_H_
#define SCORER_FLOATSCORE_H_

namespace kws {
namespace scorer {

class FloatScore {
 public:
  FloatScore() : value_(-std::numeric_limits<float>::infinity()) {}

  explicit FloatScore(float value) : value_(value) {}

  inline bool Valid() const { return std::isfinite(value_); }

  inline float Value() const { return value_; }

  inline void SetValue(float value) { value_ = value; }

  inline void MarkInvalid() {
    value_ = -std::numeric_limits<float>::infinity();
  }

  inline bool operator==(const FloatScore& other) const {
    return value_ == other.value_;
  }

  inline bool operator!=(const FloatScore& other) const {
    return value_ != other.value_;
  }

  inline bool operator<(const FloatScore& other) const {
    return value_ < other.value_;
  }

  inline bool operator>(const FloatScore& other) const {
    return value_ < other.value_;
  }

  inline bool operator<=(const FloatScore& other) const {
    return value_ < other.value_ || value_ == other.value_;
  }

  inline bool operator>=(const FloatScore& other) const {
    return value_ > other.value_ || value_ == other.value_;
  }

 private:
  float value_;
};

}  // namespace scorer
}  // namespace kws

#endif  // SCORER_FLOATSCORE_H_
