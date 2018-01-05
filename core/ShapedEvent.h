#ifndef CORE_SHAPEDEVENT_H_
#define CORE_SHAPEDEVENT_H_

#include "core/Event.h"

namespace kws {
namespace core {

template <typename Q, typename L>
class ShapedEvent : public Event<Q, L> {
 public:
  typedef typename L::Type Type;

  ShapedEvent() : Event<Q, L>() {}

  ShapedEvent(const Q& query, const L& location) :
      Event<Q, L>(query, location) {}

  virtual Type Area() const { return Event<Q, L>::location_.Area(); }
};

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

#endif  // CORE_SHAPEDEVENT_H_
