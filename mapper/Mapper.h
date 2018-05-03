#ifndef MAPPER_MAPPER_H_
#define MAPPER_MAPPER_H_

namespace kws {
namespace mapper {

template<typename I, typename O>
class Mapper {
 public:
  using InputType = I;
  using OutputType = O;

  virtual OutputType operator()(const InputType &input) = 0;
};

}  // namespace mapper
}  // namespace kws

#endif  // MAPPER_MAPPER_H_
