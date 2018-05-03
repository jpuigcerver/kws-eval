#ifndef CORE_SCOREDEVENT_H_
#define CORE_SCOREDEVENT_H_

#include <iostream>

#include "core/Event.h"

namespace kws {
namespace core {

template <typename E>
class ScoredEvent : public E {
 public:
  typedef E Base;
  typedef typename E::QType QType;
  typedef typename E::LType LType;

  ScoredEvent() {}

  explicit ScoredEvent(float s) : score_(s) {}

  ScoredEvent(const QType& query, const LType& location, float s) :
      E(query, location), score_(s) {}

  ScoredEvent(const E& event, float s) : E(event), score_(s) {}

  virtual bool operator==(const ScoredEvent& other) const {
    return score_ == other.score_ && Base::operator==(other);
  }

  virtual bool operator!=(const ScoredEvent& other) const {
    return score_ != other.score_ || Base::operator!=(other);
  }

  virtual bool operator<(const ScoredEvent& other) const {
    if (score_ != other.score_) return score_ < other.score_;
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

template <class E>
std::istream& operator>>(std::istream& is, ScoredEvent<E>& event) {
  is >> event.Query() >> event.Location() >> event.Score();
  return is;
}

template <class E>
std::ostream& operator<<(std::ostream& os, const ScoredEvent<E>& event) {
  os << event.Query() << " " << event.Location() << " " << event.Score();
  return os;
}

}  // namespace core
}  // namespace kws

#endif  // CORE_SCOREDEVENT_H_
