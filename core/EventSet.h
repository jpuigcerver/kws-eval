#ifndef CORE_EVENTSET_H_
#define CORE_EVENTSET_H_

#include <list>
#include <set>
#include <string>
#include <set>

namespace kws {
namespace core {

template <typename Q, typename L>
class Event;

template <typename Q, typename L>
class ShapedEvent;

template <class E>
class EventSet {
 public:
  typedef E EventType;
  typedef typename E::QType QType;
  typedef typename E::LType LType;
  typedef std::list<E> EventList;

  EventSet() {}

  virtual ~EventSet() {}

  template <typename I>
  EventSet(I begin, I end) {
    for (I it = begin; it != end; ++it) { Insert(*it); }
  }

  virtual void Insert(const EventType& event) {
    event_set_.emplace(event);
  }

  virtual void Clear() {
    event_set_.clear();
  }

  virtual void Remove(const EventType& event) {
    event_set_.erase(event);
  }

  virtual size_t Size() const { return event_set_.size(); }

  virtual EventList FindOverlapping(const Event<QType, LType>& event) const {
    auto it = event_set_.find(event);
    if (it != event_set_.end()) { return {*it}; }
    else { return {}; }
  }

protected:
  std::set<E> event_set_;
};


/*
// Specialization for Shaped events
template <typename Q, typename L, typename D>
class EventSet<ShapedEvent<Q, L, D>> : public BaseEventSet<D> {
 public:
  virtual EventList FindOverlapping(const ShapedEvent<QType, LType>& event) const {
    // Find all events with an intersection area > 0.
    std::vector<std::pair<typename LType::Type, E>> aux;
    auto it = events_by_query_.find(event.Query());
    if (it != events_by_query_.end()) {
      for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
        const auto intersection_area =
            it2->Location().IntersectionArea(event.Location());
        if (intersection_area > 0) {
          aux.push_back(std::make_pair(intersection_area, *it2));
        }
      }
    }
    // Sort the events by decreasing intersection area.
    std::sort(aux.begin(), aux.end(), std::greater<std::pair<typename LType::Type, E>>());
    EventList result;
    for (const auto& a : aux) { result.push_back(a.second); }
    return result;
  }
}; */

}  // namespace core
}  // namespace kws

#endif  // CORE_EVENTSET_H_
