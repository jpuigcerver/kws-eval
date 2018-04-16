#ifndef CORE_MATCH_H_
#define CORE_MATCH_H_

#include <cassert>
#include <memory>

#include "core/MatchError.h"

namespace kws {
namespace core {

template <class RE, class HE>
class Match {
 public:
  typedef RE RefEvent;
  typedef HE HypEvent;

  Match() {}

  Match(const Match& other) : error_(other.error_) {
    if (other.HasRef()) SetRef(other.GetRef());
    if (other.HasHyp()) SetHyp(other.GetHyp());
  }

  Match(float fp, float fn) : error_(fp, fn) {}

  Match(const RefEvent& ref, const HypEvent& hyp, const MatchError& error) :
      error_(error) {
    ref_.reset(new RefEvent(ref));
    hyp_.reset(new HypEvent(hyp));
  }

  inline bool HasHyp() const { return hyp_ != nullptr; }

  inline bool HasRef() const { return ref_ != nullptr; }

  inline void SetError(const MatchError& error) { error_ = error; }

  inline void SetHyp(const HypEvent& hyp) { hyp_.reset(new HypEvent(hyp)); }

  inline void SetRef(const RefEvent& ref) { ref_.reset(new RefEvent(ref)); }

  inline const MatchError& GetError() const { return error_; }

  inline const HypEvent& GetHyp() const {
    assert(hyp_ != nullptr);
    return *hyp_;
  }

  inline const RefEvent& GetRef() const {
    assert(ref_ != nullptr);
    return *ref_;
  }

  Match& operator=(const Match& other) {
    error_ = other.error_;
    ref_.release(); hyp_.release();
    if (other.HasRef()) SetRef(other.GetRef());
    if (other.HasHyp()) SetHyp(other.GetHyp());
    return *this;
  }

  Match& operator=(Match&& other) {
    error_ = std::move(other.error_);
    ref_ = std::move(other.ref_);
    hyp_ = std::move(other.hyp_);
    return *this;
  }

  bool operator==(const Match& other) const {
    return error_ == other.error_ &&
        HasRef() == other.HasRef() &&
        HasHyp() == other.HasHyp() &&
        (!HasRef() || GetRef() == other.GetRef()) &&
        (!HasHyp() || GetHyp() == other.GetHyp());
  }

  bool operator!=(const Match& other) const {
    return !(*this != other);
  }

  static Match<RE, HE> MakeFalsePositive(const HE& hyp) {
    Match<RE, HE> m;
    m.SetError(MatchError{1.0f, 0.0f});
    m.SetHyp(hyp);
    return m;
  };

  static Match<RE, HE> MakeFalseNegative(const RE& ref) {
    Match<RE, HE> m;
    m.SetError(MatchError{0.0f, 1.0f});
    m.SetRef(ref);
    return m;
  };

 private:
  std::unique_ptr<RE> ref_;
  std::unique_ptr<HE> hyp_;
  MatchError error_;
};

template <class RE, class HE>
std::ostream& operator<<(std::ostream& oss, const Match<RE, HE>& m) {
  oss << "Match[";
  if (m.HasHyp()) oss << "Hyp=" << m.GetHyp() << ", ";
  if (m.HasRef()) oss << "Ref=" << m.GetRef() << ", ";
  oss << "Error=" << m.GetError() << "]";
  return oss;
}

}  // namespace core
}  // namespace kws

#endif  // CORE_MATCH_H_
