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

  StringEventToIntMapper() : map_(new StringToIntMapper<Int>()), own_(true) {}

  StringEventToIntMapper(const StringEventToIntMapper &other)
      : map_(other.map_), own_(false) {}

  explicit StringEventToIntMapper(StringToIntMapper<Int> *map)
      : map_(map), own_(false) {}

  StringEventToIntMapper(StringToIntMapper<Int> *map, bool own)
      : map_(map), own_(own) {}

  OutputType operator()(const InputType &input) override {
    return OutputType((*map_)(input.Query()), (*map_)(input.Location()));
  }

  StringToIntMapper<Int>* GetMapper() const { return map_; }

 private:
  StringToIntMapper<Int> *map_;
  bool own_;
};

}  // namespace mapper
}  // namespace kws

#endif  // MAPPER_STRINGEVENTTOINTMAPPER_H_
