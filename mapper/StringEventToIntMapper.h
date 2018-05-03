#ifndef MAPPER_STRINGEVENTTOINTMAPPER_H_
#define MAPPER_STRINGEVENTTOINTMAPPER_H_

#include <string>

#include "core/Event.h"
#include "mapper/Mapper.h"
#include "mapper/StringToIntMapper.h"

namespace kws {
namespace mapper {

using kws::core::Event;

template<typename Int>
class StringEventToIntMapper :
    public Mapper<Event<std::string, std::string>, Event<Int, Int>> {
 public:
  typedef Event<std::string, std::string> InputType;
  typedef Event<Int, Int> OutputType;
  StringEventToIntMapper(StringToIntMapper<Int> *map) : map_(map) {}

  OutputType operator()(const InputType &input) override {
    return OutputType((*map_)(input.Query()), (*map_)(input.Location()));
  }

 private:
  StringToIntMapper<Int> *map_;
};

}  // namespace mapper
}  // namespace kws

#endif  // MAPPER_STRINGEVENTTOINTMAPPER_H_
