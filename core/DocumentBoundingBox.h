#ifndef CORE_DOCUMENTBOUNDINGBOX_H_
#define CORE_DOCUMENTBOUNDINGBOX_H_

#include <limits>

#include "core/BoundingBox.h"

namespace kws {
namespace core {

template <typename T>
struct DocumentBoundingBox : public BoundingBox<T> {
  std::string document;

  DocumentBoundingBox() {}

  DocumentBoundingBox(const std::string& p,
                      const T& x, const T& y, const T& w, const T& h) :
      BoundingBox<T>(x, y, w, h), document(p) { }

  inline bool operator==(const DocumentBoundingBox& other) const {
    return document == other.document && BoundingBox<T>::operator==(other);
  }

  inline bool operator!=(const DocumentBoundingBox& other) const {
    return document != other.document || BoundingBox<T>::operator!=(other);
  }

  inline bool operator<(const DocumentBoundingBox& other) const {
    if (document != other.document) return document < other.document;
    return BoundingBox<T>::operator<(other);
  }

  inline bool operator>(const DocumentBoundingBox& other) const {
    return (other < *this);
  }

  inline bool operator<=(const DocumentBoundingBox& other) const {
    return !(*this > other);
  }

  inline bool operator>=(const DocumentBoundingBox& other) const {
    return !(*this < other);
  }

  inline T IntersectionArea(const DocumentBoundingBox& other) const {
    if (document == other.document)
      return BoundingBox<T>::IntersectionArea(other);
    else
      return 0;
  }

  inline T UnionArea(const DocumentBoundingBox& other) const {
    return BoundingBox<T>::Area() + other.Area() - IntersectionArea(other);
  }
};

template <typename T>
std::istream& operator>>(std::istream& is, DocumentBoundingBox<T>& bb) {
  is >> bb.document >> static_cast<BoundingBox<T>&>(bb);
  return is;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const DocumentBoundingBox<T>& bb) {
  os << bb.document << " " << static_cast<const BoundingBox<T>&>(bb);
  return os;
}

template <typename T>
inline T IntersectionArea(const DocumentBoundingBox<T>& a,
                          const DocumentBoundingBox<T>& b) {
  return a.IntersectionArea(b);
}

template <typename T>
inline T UnionArea(const DocumentBoundingBox<T>& a,
                   const DocumentBoundingBox<T>& b) {
  return a.UnionArea(b);
}

}  // namespace core
}  // namespace kws

#endif  // CORE_DOCUMENTBOUNDINGBOX_H_
