#ifndef CORE_DUMMYLOCATION_H
#define CORE_DUMMYLOCATION_H

namespace kws {
namespace core {
namespace testing {

class DummyLocation {
public:
  typedef int Type;

  DummyLocation() : id_(0) {}

  DummyLocation(int v) : id_(v) {}

  inline bool operator==(const DummyLocation &other) const {
    return id_ == other.id_;
  }

  inline bool operator!=(const DummyLocation &other) const {
    return id_ != other.id_;
  }

  inline bool operator<(const DummyLocation &other) const {
    return id_ < other.id_;
  }

  inline bool operator>(const DummyLocation &other) const {
    return id_ > other.id_;
  }

  inline bool operator<=(const DummyLocation &other) const {
    return id_ <= other.id_;
  }

  inline bool operator>=(const DummyLocation &other) const {
    return id_ >= other.id_;
  }

  inline int Area() const {
    return 0;
  }

  inline int IntersectionArea(const DummyLocation &other) const {
    return 0;
  }

  inline int UnionArea(const DummyLocation &other) const {
    return 0;
  }

private:
  int id_;

  friend std::ostream& operator<<(std::ostream& os, const DummyLocation& m);
};

std::ostream& operator<<(std::ostream& os, const DummyLocation& m) {
  os << "DummyLocation[" << m.id_ << "]";
  return os;
}

std::istream& operator>>(std::istream& is, DummyLocation& m) {
  std::string aux;
  is >>  aux;
  return is;
}

}  // namespace kws
}  // namespace core
}  // namespace testing

#endif // CORE_DUMMYLOCATION_H
