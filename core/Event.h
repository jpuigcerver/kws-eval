#ifndef CORE_EVENT_H_
#define CORE_EVENT_H_

#include <string>
#include <iostream>

namespace kws {
namespace core {

template <typename L>
struct Event {
  typedef L Location;
  std::string query;
  L location;

  Event() {}

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
};

template <typename L>
std::istream& operator>>(std::istream& is, Event<L>& event) {
  is >> event.query >> event.location;
  while(is.good() && is.get() != '\n') { }
  return is;
}

template <typename L>
std::ostream& operator<<(std::ostream& os, const Event<L>& event) {
  os << event.query << " " << event.location << std::endl;
  return os;
}

}  // namespace core
}  // namespace kws

#endif  // CORE_EVENT_H_
