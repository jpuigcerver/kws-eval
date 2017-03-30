#ifndef CORE_SCOREDEVENT_H_
#define CORE_SCOREDEVENT_H_

#include <iostream>

#include "core/Event.h"

namespace kws {
namespace core {

template <typename L>
struct ScoredEvent : public Event<L> {
  float score;

  ScoredEvent() {}

  ScoredEvent(const std::string& q, const L& loc, float s) :
      Event<L>(q, loc), score(s) {}

  inline bool operator==(const ScoredEvent& other) const {
    return score == other.score && Event<L>::operator==(other);
  }

  inline bool operator!=(const ScoredEvent& other) const {
    return score != other.score || Event<L>::operator!=(other);
  }

  inline bool operator<(const ScoredEvent& other) const {
    if (score < other.score) return true;
    else if (score > other.score) return false;
    return Event<L>::operator<(other);
  }

  inline bool operator>(const ScoredEvent& other) const {
    if (score > other.score) return true;
    else if (score < other.score) return false;
    return Event<L>::operator>(other);
  }

  inline bool operator<=(const ScoredEvent& other) const {
    return (*this < other) || (*this == other);
  }

  inline bool operator>=(const ScoredEvent& other) const {
    return (*this > other) || (*this == other);
  }
};

template <typename L>
std::istream& operator>>(std::istream& is, ScoredEvent<L>& event) {
  is >> event.query >> event.location >> event.score;
  while(is.good() && is.get() != '\n') {}
  return is;
}

template <typename L>
std::ostream& operator<<(std::ostream& os, const ScoredEvent<L>& event) {
  os << event.query << " " << event.location << " " << event.score;
  return os;
}

}  // namespace core
}  // namespace kws

#endif  // CORE_SCOREDEVENT_H_
