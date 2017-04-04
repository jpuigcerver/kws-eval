#ifndef CORE_EVENT_H_
#define CORE_EVENT_H_

#include <string>
#include <iostream>

namespace kws {
namespace core {

template <typename L>
struct Event {
  typedef L Location;
  typedef typename Location::Type Type;
  std::string query;
  L location;

  Event() {}

  virtual ~Event() {}

  Event(const std::string& q, const Location& loc) :
      query(q), location(loc) {}

  inline bool operator==(const Event& other) const {
    return query == other.query && location == other.location;
  }

  inline bool operator!=(const Event& other) const {
    return query != other.query || location != other.location;
  }

  inline bool operator<(const Event& other) const {
    return (query == other.query && location < other.location) ||
        (query < other.query);
  }

  inline bool operator>(const Event& other) const {
    return (query == other.query && location > other.location) ||
        (query > other.query);
  }

  inline bool operator<=(const Event& other) const {
    return (*this < other) || (*this == other);
  }

  inline bool operator>=(const Event& other) const {
    return (*this > other) || (*this == other);
  }

  inline Type Area() const { return location.Area(); }
};

template <typename L>
std::istream& operator>>(std::istream& is, Event<L>& event) {
  // Skip lines started with #, these are comments.
  is >> std::ws;
  while (is.good() && is.peek() == '#') {
    while (is.good() && is.get() != '\n') {}
    is >> std::ws;
  }
  is >> event.query >> event.location >> std::ws;
  return is;
}

template <typename L>
std::ostream& operator<<(std::ostream& os, const Event<L>& event) {
  os << event.query << " " << event.location;
  return os;
}

template <class E1, class E2>
typename E1::Type IntersectionArea(const E1& a, const E2& b) {
  return IntersectionArea(a.location, b.location);
}

template <class E1, class E2>
typename E1::Type UnionArea(const E1& a, const E2& b) {
  return UnionArea(a.location, b.location);
}

}  // namespace core
}  // namespace kws

#endif  // CORE_EVENT_H_
