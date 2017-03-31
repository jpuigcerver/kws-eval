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
    const T dy = y - other.y, dx = x - other.x;
    const T dh = h - other.h, dw = w - other.w;
    if (dy != 0) return dy < 0;
    else if (dx != 0) return dx < 0;
    else if (dw != 0) return dw < 0;
    else return dh < 0;
  }

  inline bool operator>(const BoundingBox& other) const {
    const T dy = y - other.y, dx = x - other.x;
    const T dh = h - other.h, dw = w - other.w;
    if (dy != 0) return dy > 0;
    else if (dx != 0) return dx > 0;
    else if (dw != 0) return dw > 0;
    else return dh > 0;
  }

  inline bool operator<=(const BoundingBox& other) const {
    return (*this < other) || (*this == other);
  }

  inline bool operator>=(const BoundingBox& other) const {
    return (*this > other) || (*this == other);
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

  inline float CenterDistance(const BoundingBox& other) const {
    const float a_x = (x + w) / 2.0, a_y = (y + h) / 2.0;
    const float b_x = (other.x + other.w) / 2.0,
        b_y = (other.y + other.h) / 2.0;
    const float c_x = a_x - b_x, c_y = a_y - b_y;
    return sqrt(c_x * c_x + c_y * c_y);
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
