#ifndef CORE_MOCKLOCATION_H_
#define CORE_MOCKLOCATION_H_

#include <gmock/gmock.h>

namespace kws {
namespace core {
namespace testing {

template <typename T>
class MockLocation {
 public:
  typedef int Type;

  MockLocation() {
    std::ostringstream oss;
    oss << "MockLocation[" << std::hex << reinterpret_cast<size_t>(this) << "]";
    str_ = oss.str();
  }

  virtual ~MockLocation() {}

  // Comparison operators
  MOCK_CONST_METHOD1_T(Equals, bool(const MockLocation<T>&));
  MOCK_CONST_METHOD1_T(NotEquals, bool(const MockLocation<T>&));
  MOCK_CONST_METHOD1_T(Lower, bool(const MockLocation<T>&));
  MOCK_CONST_METHOD1_T(Greater, bool(const MockLocation<T>&));
  MOCK_CONST_METHOD1_T(LowerEq, bool(const MockLocation<T>&));
  MOCK_CONST_METHOD1_T(GreaterEq, bool(const MockLocation<T>&));

  // Geometry methods
  MOCK_CONST_METHOD0_T(Area, T());
  MOCK_CONST_METHOD1_T(IntersectionArea, T(const MockLocation<T>&));
  MOCK_CONST_METHOD1_T(UnionArea, T(const MockLocation<T>&));
  MOCK_CONST_METHOD1_T(CenterDistance, float(const MockLocation<T>&));

  bool operator==(const MockLocation<T>& other) const {
    return this->Equals(other);
  }

   bool operator!=(const MockLocation<T>& other) const {
    return this->NotEquals(other);
  }

  bool operator<(const MockLocation<T>& other) const {
    return this->Lower(other);
  }

  bool operator>(const MockLocation<T>& other) const {
    return this->Greater(other);
  }

  bool operator<=(const MockLocation<T>& other) const {
    return this->LowerEq(other);
  }

  bool operator>=(const MockLocation<T>& other) const {
    return this->GreaterEq(other);
  }

  const std::string& String() const {
    return str_;
  }

  std::string& String() {
    return str_;
  }

 private:
  std::string str_;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const MockLocation<T>& m) {
  os << m.String();
  return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, MockLocation<T>& m) {
  is >> m.String();
  return is;
}

template <typename T>
T IntersectionArea(const MockLocation<T>& a, const MockLocation<T>& b) {
  return a.IntersectionArea(b);
}

template <typename T>
T UnionArea(const MockLocation<T>& a, const MockLocation<T>& b) {
  return a.UnionArea(b);
}



}  // namespace testing
}  // namespace core
}  // namespace kws

#endif  // CORE_MOCKLOCATION_H_
