#ifndef CORE_MOCKEVENT_H_
#define CORE_MOCKEVENT_H_

namespace kws {
namespace core {
namespace testing {

class MockEvent {
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
