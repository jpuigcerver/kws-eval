#ifndef MAPPER_STRINGTOINTMAPPER_H_
#define MAPPER_STRINGTOINTMAPPER_H_

#include <string>
#include <unordered_map>

#include "mapper/Mapper.h"

namespace kws {
namespace mapper {

template<typename Int, typename M = std::unordered_map<std::string, Int>>
class StringToIntMapper : public Mapper<std::string, Int> {
 public:
  typedef M Impl;
  typedef typename Mapper<std::string, Int>::InputType InputType;
  typedef typename Mapper<std::string, Int>::OutputType OutputType;

  StringToIntMapper() : impl_(new Impl), own_(true) {}

  explicit StringToIntMapper(Impl *impl) : impl_(impl), own_(false) {}

  StringToIntMapper(Impl *impl, bool own) : impl_(impl), own_(own) {}

  ~StringToIntMapper() {
    if (own_) {
      delete impl_;
    }
  }

  OutputType operator()(const InputType &input) override {
    const auto it = impl_->find(input);
    if (it != impl_->end()) {
      return it->second;
    } else {
      assert(impl_->size() < std::numeric_limits<Int>::max());
      return impl_->emplace_hint(it, input, impl_->size())->second;
    }
  }

 private:
  Impl *impl_;
  bool own_;
};

}  // namespace mapper
}  // namespace kws

#endif  // MAPPER_STRINGTOINTMAPPER_H_
