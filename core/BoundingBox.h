#ifndef CORE_BOUNDINGBOX_H_
#define CORE_BOUNDINGBOX_H_

#include <cmath>
#include <iostream>

namespace kws {
namespace core {

template <typename T>
struct BoundingBox {
  typedef T Type;
  T x, y, w, h;

  BoundingBox() {}

  BoundingBox(const T& x, const T& y, const T& w, const T& h) :
      x(x), y(y), w(w), h(h) { }

  inline bool operator==(const BoundingBox& other) const {
    return x == other.x && y == other.y && w == other.w && h == other.h;
  }

  inline bool operator!=(const BoundingBox& other) const {
    return x != other.x || y != other.y || w != other.w || h != other.h;
  }

  inline bool operator<(const BoundingBox& other) const {
    if (x != other.x) return x < other.x;
    if (y != other.y) return y < other.y;
    if (w != other.w) return w < other.w;
    return h < other.h;
  }

  inline bool operator>(const BoundingBox& other) const {
    return (other < *this);
  }

  inline bool operator<=(const BoundingBox& other) const {
    return !(*this > other);
  }

  inline bool operator>=(const BoundingBox& other) const {
    return !(*this < other);
  }

  inline T Area() const {
    return w * h;
  }

  inline T IntersectionArea(const BoundingBox& other) const {
    const T x1 = std::max<T>(x, other.x);
    const T x2 = std::min<T>(x + w, other.x + other.w);
    const T y1 = std::max<T>(y, other.y);
    const T y2 = std::min<T>(y + h, other.y + other.h);
    if (x1 > x2 || y1 > y2) return 0;
    else return (x2 - x1) * (y2 - y1);
  }

  inline T UnionArea(const BoundingBox& other) const {
    return Area() + other.Area() - IntersectionArea(other);
  }
};

template <typename T>
std::istream& operator>>(std::istream& is, BoundingBox<T>& bb) {
  is >> bb.x >> bb.y >> bb.w >> bb.h;
  return is;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const BoundingBox<T>& bb) {
  os << bb.x << " " << bb.y << " " << bb.w << " " << bb.h;
  return os;
}

template <typename T>
inline T IntersectionArea(const BoundingBox<T>& a, const BoundingBox<T>& b) {
  return a.IntersectionArea(b);
}

template <typename T>
inline T UnionArea(const BoundingBox<T>& a, const BoundingBox<T>& b) {
  return a.UnionArea(b);
}

}  // namespace core
}  // namespace kws

#endif  // CORE_BOUNDINGBOX_H_
