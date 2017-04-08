#ifndef CORE_DOCUMENTBOUNDINGBOXEVENTSET_H
#define CORE_DOCUMENTBOUNDINGBOXEVENTSET_H

#include <list>
#include <set>
#include <string>
#include <unordered_map>

#include "core/DocumentBoundingBox.h"

namespace kws {
namespace core {

template <class Q, class T>
class Event;

template <class E>
class EventSet;

// Specialization of the EventSet when the event objects are templated with
// the DocumentBoundingBox location

template <template <class, class> class E, class Q, typename T>
class EventSet<E<Q, DocumentBoundingBox<T>>> {
 public:
  typedef E<Q, DocumentBoundingBox<T>> EventType;
  typedef Q QType;
  typedef DocumentBoundingBox<T> LType;
  typedef std::list<EventType> EventList;
  typedef std::set<EventType> EventSetInternal;

  EventSet() : size_(0) {}

  virtual ~EventSet() {}

  template <typename I>
  EventSet(I begin, I end) {
    for (I it = begin; it != end; ++it) { Insert(*it); }
  }

  virtual void Insert(const EventType &event) {
    DocumentToEventsMap &p = documents_by_query_.emplace(
            event.Query(), DocumentToEventsMap()).first->second;
    EventSetInternal &l = p.emplace(
            event.Location().document, EventSetInternal()).first->second;
    if(l.insert(event).second) {
      ++size_;
    }
  }

  virtual void Clear() {
    documents_by_query_.clear();
    size_ = 0;
  }

  virtual void Remove(const EventType &event) {
    auto p = documents_by_query_.find(event.Query());
    if (p != documents_by_query_.end()) {
      auto l = p->second.find(event.Location().document);
      if (l != p->second.end()) {
        size_ -= l->second.erase(event);
      }
    }
  }

  virtual EventList FindOverlapping(const Event<QType, LType> &event) const {
    // Find all events with an intersection area > 0.
    std::vector<std::pair<typename LType::Type, EventType>> aux;
    auto p = documents_by_query_.find(event.Query());
    if (p != documents_by_query_.end()) {
      auto l = p->second.find(event.Location().document);
      if (l != p->second.end()) {
        for (auto it = l->second.begin(); it != l->second.end(); ++it) {
          const auto intersection_area =
              it->Location().IntersectionArea(event.Location());
          if (intersection_area > 0) {
            aux.push_back(std::make_pair(intersection_area, *it));
          }
        }
      }
    }
    // Sort the events by decreasing intersection area.
    std::sort(aux.begin(), aux.end(),
              std::greater<std::pair<typename LType::Type, EventType>>());
    EventList result;
    for (const auto& a : aux) { result.push_back(a.second); }
    return result;
  }

  virtual const size_t& Size() const { return size_; }

 private:
  typedef std::unordered_map<std::string, EventSetInternal> DocumentToEventsMap;
  typedef std::unordered_map<QType, DocumentToEventsMap> QueryToDocumentsMap;
  QueryToDocumentsMap documents_by_query_;
  size_t size_;
};

}  // namespace core
}  // namespace kws

#endif //CORE_DOCUMENTBOUNDINGBOXEVENTSET_H
