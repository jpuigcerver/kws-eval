#ifndef CORE_EVENTSET_H_
#define CORE_EVENTSET_H_

#include <list>
#include <set>
#include <string>
#include <unordered_map>

namespace kws {
namespace core {

template <typename Q, typename L>
class Event;

// Generic implementation of a set of Event objects
template <class E>
class EventSet {
 public:
  typedef E EventType;
  typedef typename E::QType QType;
  typedef typename E::LType LType;
  typedef std::list<E> EventList;
  typedef std::set<E> EventSetInternal;

  EventSet() : size_(0) {}

  virtual ~EventSet() {}

  template <typename I>
  EventSet(I begin, I end) {
    for (I it = begin; it != end; ++it) { Insert(*it); }
  }

  virtual void Insert(const EventType& event) {
    EventSetInternal& qe =  events_by_query_.emplace(
        event.Query(), EventSetInternal()).first->second;
    if (qe.emplace(event).second) {
      ++size_;
    }
  }

  virtual void Clear() {
    events_by_query_.clear();
    size_ = 0;
  }

  virtual void Remove(const EventType& event) {
    auto it = events_by_query_.find(event.Query());
    if (it != events_by_query_.end()) {
      size_ -= it->second.erase(event);
    }
  }

  virtual EventList FindOverlapping(const Event<QType, LType>& event) const {
    // Find all events with an intersection area > 0.
    std::vector<std::pair<typename LType::Type, E>> aux;
    auto it = events_by_query_.find(event.Query());
    if (it != events_by_query_.end()) {
      for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
        const auto intersection_area =
            it2->Location().IntersectionArea(event.Location());
        if (intersection_area > 0) {
          aux.push_back(std::make_pair(intersection_area, event));
        }
      }
    }
    // Sort the events by decreasing intersection area.
    std::sort(aux.begin(), aux.end(), std::greater<std::pair<typename LType::Type, E>>());
    EventList result;
    for (const auto& a : aux) { result.push_back(a.second); }
    return result;
  }

  virtual const size_t& Size() const { return size_; }

private:
  std::unordered_map<QType, EventSetInternal> events_by_query_;
  size_t size_;
};

}  // namespace core
}  // namespace kws

#endif  // CORE_EVENTSET_H_
