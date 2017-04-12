#ifndef CMD_ARGUMENT_H
#define CMD_ARGUMENT_H

#include "cmd/Option.h"
#include "core/TypeInfo.h"

namespace kws {
namespace cmd {

using kws::core::TypeInfo;

template <typename T>
class Argument : public Option<T> {
 public:
  using Option<T>::Option;
  using Option<T>::Parse;

  std::string Help(size_t w = 0) const override {
    std::ostringstream oss;
    oss << std::setw(w) << std::left << name_ << " : " << help_
        << " (type = " << TypeInfo<T>::Name() << ")";
    return oss.str();
  }

 protected:
  using Option<T>::name_;
  using Option<T>::help_;
  using Option<T>::value_;
};

template <>
class Argument<bool> : public Option<bool> {
 public:
  using Option<bool>::Option;

  std::string Help(size_t w = 0) const override {
    std::ostringstream oss;
    oss << std::setw(w) << std::left << name_ << " : " << help_
        << " (type = " << TypeInfo<bool>::Name() << ")";
    return oss.str();
  }

  bool Parse(const std::string& str) override {
    if (str == "true" || str == "1") {
      *value_ = true;
      return true;
    } else if (str == "false" || str == "0") {
      *value_ = false;
      return true;
    } else {
      return false;
    }
  }

 protected:
  using Option<bool>::name_;
  using Option<bool>::help_;
  using Option<bool>::value_;
};

}  // namespace cmd
}  // namespace kws

#endif // CMD_ARGUMENT_H
