#ifndef CORE_MOCKEVENT_H_
#define CORE_MOCKEVENT_H_

#include <string>

#include "core/Event.h"
#include "core/MockLocation.h"

namespace kws {
namespace core {
namespace testing {

class MockEvent : public Event<std::string, MockLocation<int>> {
 public:
  MockEvent() {}
  virtual ~MockEvent() {}
  MOCK_CONST_METHOD0(Area, int());
  MOCK_CONST_METHOD1(IntersectionArea, int(const MockEvent&));
  MOCK_CONST_METHOD1(UnionArea, int(const MockEvent&));
};

int IntersectionArea(const MockEvent& a, const MockEvent& b) {
  return a.IntersectionArea(b);
}

int UnionArea(const MockEvent& a, const MockEvent& b) {
  return a.UnionArea(b);
}

}  // namespace testing
}  // namespace core
}  // namespace kws

#endif  // MOCK_EVENT_H_
