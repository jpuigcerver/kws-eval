#ifndef CORE_EVENTSET_H_
#define CORE_EVENTSET_H_

#include <list>
#include <string>
#include <unordered_map>
#include "core/Event.h"

namespace kws {
namespace core {

// Generic implementation of a set of Event objects
template <class E>
class EventSet {
 public:
  typedef typename E::Location Location;
  typedef std::list<E> EventList;

  EventSet() {}

  template <typename I>
  EventSet(I begin, I end) {
    for (I it = begin; it != end; ++it) { Insert(*it); }
  }

  void Insert(const E& event) {
    EventList& l =
        events_by_query_.insert(make_pair(event.query, EventList()))
        .first->second;
    l.push_back(event);
  }

  void Remove(const E& event) {
    auto it = events_by_query_.find(event.query);
    if (it != events_by_query_.end()) {
      it->remove(event);
    }
  }

  EventList FindOverlapping(const Event<Location>& event) const {
    EventList result;
    auto it = events_by_query_.find(event.query);
    if (it != events_by_query_.end()) {
      for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
        if (it2->location.IntersectionArea(event.location) > 0)
          result.push_back(event);
      }
    }
    return result;
  }

 private:
  std::unordered_map<std::string, EventList> events_by_query_;
};

// Specialization of the EventSet when the Event objects are templated with
// the DocumentBoundingBox location
template <template<class> class E, typename T>
class EventSet<E<DocumentBoundingBox<T>>> {
public:
  typedef DocumentBoundingBox<T> Location;
  typedef std::list<E<Location>> EventList;

  EventSet() {}

  template <typename I>
  EventSet(I begin, I end) {
    for (I it = begin; it != end; ++it) { Insert(*it); }
  }

  void Insert(const E<Location>& event) {
    DocumentToEventsMap& p =
        documents_by_query_.insert(make_pair(event.query, DocumentToEventsMap()))
        .first->second;
    EventList& l = p.insert(make_pair(event.query, EventList())).first->second;
    l.push_back(event);
  }

  void Remove(const E<Location>& event) {
    auto p = documents_by_query_.find(event.query);
    if (p != documents_by_query_.end()) {
      auto l = p->second.find(event.location.document);
      if (l != p->second.end()) {
        l->second.remove(event);
      }
    }
  }

  EventList FindOverlapping(const Event<Location>& event) const {
    EventList result;
    auto p = documents_by_query_.find(event.query);
    if (p != documents_by_query_.end()) {
      auto l = p->second.find(event.location.document);
      if (l != p->second.end()) {
        for (auto it = l->second.begin(); it != l->second.end(); ++it) {
          result.push_back(event);
        }
      }
    }
    return result;
  }

private:
  typedef std::unordered_map<std::string, EventList> DocumentToEventsMap;
  typedef std::unordered_map<std::string, DocumentToEventsMap> QueryToDocumentsMap;
  QueryToDocumentsMap documents_by_query_;
};

}  // namespace core
}  // namespace kws

#endif  // CORE_EVENTSET_H_
