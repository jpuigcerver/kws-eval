#ifndef MAPPER_STRINGTOINTMAPPER_H_
#define MAPPER_STRINGTOINTMAPPER_H_

#include <exception>
#include <limits>
#include <string>
#include <unordered_map>

#include "mapper/Mapper.h"

namespace kws {
namespace mapper {

template<typename Int, typename M = std::unordered_map<std::string, Int>>
class StringToIntMapper : public Mapper<std::string, Int> {
 public:
  static_assert(std::is_integral<Int>::value, "Integral type required");

  typedef M Impl;
  typedef typename Mapper<std::string, Int>::InputType InputType;
  typedef typename Mapper<std::string, Int>::OutputType OutputType;

  StringToIntMapper() : impl_(new Impl), own_(true) {}

  StringToIntMapper(const StringToIntMapper &other)
      : impl_(other.impl_), own_(false) {}

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
      if (impl_->size() < kMaxSize) {
        return impl_->emplace_hint(it, input, impl_->size())->second;
      } else {
        throw std::range_error(
            "Max size reached (" + std::to_string(kMaxSize) + ")");
      }
    }
  }

 private:
  static constexpr size_t kMaxSize =
      1 + static_cast<size_t>(std::numeric_limits<Int>::max());

  Impl *impl_;
  bool own_;
};

}  // namespace mapper
}  // namespace kws

#endif  // MAPPER_STRINGTOINTMAPPER_H_
