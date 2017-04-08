#ifndef CORE_EVENT_H_
#define CORE_EVENT_H_

#include <string>
#include <iostream>

namespace kws {
namespace core {

template <typename Q, typename L>
class Event {
 public:
  typedef Q QType;
  typedef L LType;
  typedef typename LType::Type Type;
  typedef Event<Q, L> Base;

  Event() {}

  virtual ~Event() {}

  Event(const QType& query, const LType& location) :
      query_(query), location_(location) {}

  virtual const QType& Query() const { return query_; }

  virtual QType& Query() { return query_; }

  virtual const LType& Location() const { return location_; }

  virtual LType& Location() { return location_; }

  virtual bool operator==(const Event& other) const {
    return query_ == other.query_ && location_ == other.location_;
  }

  virtual bool operator!=(const Event& other) const {
    return query_ != other.query_ || location_ != other.location_;
  }

  virtual bool operator<(const Event& other) const {
    if (query_ != other.query_) return query_ < other.query_;
    return location_ < other.location_;
  }

  virtual bool operator>(const Event& other) const {
    return (other < *this);
  }

  virtual bool operator<=(const Event& other) const {
    return !(*this > other);
  }

  virtual bool operator>=(const Event& other) const {
    return !(*this < other);
  }

  virtual Type Area() const { return location_.Area(); }

 protected:
  QType query_;
  LType location_;
};

template <typename Q, typename L>
std::istream& operator>>(std::istream& is, Event<Q, L>& event) {
  is >> event.Query() >> event.Location();
  return is;
}

template <typename Q, typename L>
std::ostream& operator<<(std::ostream& os, const Event<Q, L>& event) {
  os << event.Query() << " " << event.Location();
  return os;
}

template <class E1, class E2>
typename E1::Type IntersectionArea(const E1& a, const E2& b) {
  return IntersectionArea(a.Location(), b.Location());
}

template <class E1, class E2>
typename E1::Type UnionArea(const E1& a, const E2& b) {
  return UnionArea(a.Location(), b.Location());
}

}  // namespace core
}  // namespace kws

#endif  // CORE_EVENT_H_
