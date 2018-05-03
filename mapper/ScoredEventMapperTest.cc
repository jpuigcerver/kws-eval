#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "core/Event.h"
#include "mapper/ScoredEventMapper.h"

using kws::mapper::Mapper;
using kws::mapper::ScoredEventMapper;

template <typename I, typename O>
class MockEventMapper : public Mapper<I, O> {
 public:
  typedef typename Mapper<I, O>::InputType InputType;
  typedef typename Mapper<I, O>::OutputType OutputType;

  MOCK_METHOD1_T(Call, OutputType(const InputType&));

  OutputType operator()(const InputType &input) override {
    return Call(input);
  }
};

TEST(ScoredEventMapperTest, Test) {
  typedef kws::core::Event<int, int> Event;
  typedef kws::core::ScoredEvent<Event> ScoredEvent;

  MockEventMapper<Event, Event> em;
  EXPECT_CALL(em, Call(Event(1, 1)))
      .WillRepeatedly(::testing::Return(Event(2, 2)));
  EXPECT_CALL(em, Call(Event(2, 2)))
      .WillRepeatedly(::testing::Return(Event(3, 3)));

  ScoredEventMapper<MockEventMapper<Event, Event>> sem(&em);
  EXPECT_EQ(sem(ScoredEvent(1, 1, 3.2)), ScoredEvent(2, 2, 3.2));
  EXPECT_EQ(sem(ScoredEvent(1, 1, 7.2)), ScoredEvent(2, 2, 7.2));
  EXPECT_EQ(sem(ScoredEvent(2, 2, 7.2)), ScoredEvent(3, 3, 7.2));
}
