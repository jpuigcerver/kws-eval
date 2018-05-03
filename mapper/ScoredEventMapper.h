#ifndef MAPPER_SCOREDEVENTMAPPER_H_
#define MAPPER_SCOREDEVENTMAPPER_H_

#include "core/ScoredEvent.h"
#include "mapper/Mapper.h"

namespace kws {
namespace mapper {

using kws::core::ScoredEvent;

template<typename M>
class ScoredEventMapper : public Mapper<ScoredEvent<typename M::InputType>,
                                        ScoredEvent<typename M::OutputType>> {
 public:
  typedef M EventMapper;
  typedef ScoredEvent<typename M::InputType> InputType;
  typedef ScoredEvent<typename M::OutputType> OutputType;

  explicit ScoredEventMapper(EventMapper *mapper) : map_(mapper) {}

  OutputType operator()(const InputType &input) override {
    return OutputType((*map_)(input), input.Score());
  }

 private:
  EventMapper *map_;
};

}  // namespace mapper
}  // namespace kws


#endif  // MAPPER_SCOREDEVENTMAPPER_H_
