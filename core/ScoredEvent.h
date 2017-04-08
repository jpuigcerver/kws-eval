#ifndef CORE_SCOREDEVENT_H_
#define CORE_SCOREDEVENT_H_

#include <iostream>

#include "core/Event.h"

namespace kws {
namespace core {

template <typename Q, typename L>
class ScoredEvent : public Event<Q, L> {
 public:
  typedef Event<Q, L> Base;

  ScoredEvent() {}

  explicit ScoredEvent(float s) : score_(s) {}

  ScoredEvent(const Q& query, const L& location, float s) :
      Event<Q, L>(query, location), score_(s) {}

  virtual bool operator==(const ScoredEvent& other) const {
    return score_ == other.score_ && Base::operator==(other);
  }

  virtual bool operator!=(const ScoredEvent& other) const {
    return score_ != other.score_ || Base::operator!=(other);
  }

  virtual bool operator<(const ScoredEvent& other) const {
    if (score_ < other.score_) return true;
    else if (score_ > other.score_) return false;
    return Base::operator<(other);
  }

  virtual bool operator>(const ScoredEvent& other) const {
    return (other < *this);
  }

  virtual bool operator<=(const ScoredEvent& other) const {
    return !(other > *this);
  }

  virtual bool operator>=(const ScoredEvent& other) const {
    return !(*this < other);
  }

  virtual float Score() const { return score_; }

  virtual float& Score() { return score_; }

 private:
  float score_;
};

template <class Q, class L>
std::istream& operator>>(std::istream& is, ScoredEvent<Q, L>& event) {
  is >> event.Query() >> event.Location() >> event.Score();
  return is;
}

template <class Q, class L>
std::ostream& operator<<(std::ostream& os, const ScoredEvent<Q, L>& event) {
  os << event.Query() << " " << event.Location() << " " << event.Score();
  return os;
}

}  // namespace core
}  // namespace kws

#endif  // CORE_SCOREDEVENT_H_
