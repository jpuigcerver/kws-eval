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

  DocumentBoundingBox(const T& x, const T& y, const T& w, const T& h,
              const std::string& p) :
      BoundingBox<T>(x, y, w, h), document(p) { }

  inline bool operator==(const DocumentBoundingBox& other) const {
    return document == other.document && BoundingBox<T>::operator==(other);
  }

  inline bool operator!=(const DocumentBoundingBox& other) const {
    return document != other.document || BoundingBox<T>::operator!=(other);
  }

  inline bool operator<(const DocumentBoundingBox& other) const {
    return (document == document && BoundingBox<T>::operator<(other)) ||
        document < other.document;
  }

  inline bool operator>(const DocumentBoundingBox& other) const {
    return (document == document && BoundingBox<T>::operator>(other)) ||
        document > other.document;
  }

  inline T IntersectionArea(const DocumentBoundingBox& other) const {
    if (document == other.document) return BoundingBox<T>::IntersectionArea(other);
    else return 0;
  }

  inline T UnionArea(const DocumentBoundingBox& other) const {
    return BoundingBox<T>::Area() + other.Area() - IntersectionArea(other);
  }

  inline float CenterDistance(const DocumentBoundingBox& other) const {
    if (document == other.document) return BoundingBox<T>::CenterDistance(other);
    else return std::numeric_limits<float>::infinity();
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

}  // namespace core
}  // namespace kws

#endif  // CORE_DOCUMENTBOUNDINGBOX_H_
