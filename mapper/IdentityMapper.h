#ifndef MAPPER_IDENTITYMAPPER_H_
#define MAPPER_IDENTITYMAPPER_H_

#include "mapper/Mapper.h"

namespace kws {
namespace mapper {

template<typename T>
class IdentityMapper : public Mapper<T, T> {
 public:
  typedef T InputType;
  typedef T OutputType;

  OutputType operator()(const InputType &input) override { return input; }
};

}  // namespace mapper
}  // namespace kws

#endif  // MAPPER_IDENTITYMAPPER_H_