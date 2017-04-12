#ifndef CMD_OPTION_H
#define CMD_OPTION_H

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

#include "core/TypeInfo.h"

namespace kws {
namespace cmd {

using kws::core::TypeInfo;

class BaseOption {
 public:
  virtual ~BaseOption() {}

  virtual bool IsBoolean() const = 0;

  virtual std::string Help(size_t w = 0) const = 0;

  virtual bool Parse(const std::string &str) = 0;

  virtual const std::string& Name() const = 0;
};

template<typename T>
class Option : public BaseOption {
 public:
  Option(const std::string &name, const std::string &help, T *value) :
      name_(name), help_(help), value_(value) {
    assert(value_ != nullptr);
  }

  ~Option() override {}

  std::string Help(size_t w = 0) const override {
    std::ostringstream oss;
    oss << std::setw(w) << std::left << name_ << " : " << help_
        << " (type = " << TypeInfo<T>::Name()
        << ", value = \"" << *value_ << "\")";
    return oss.str();
  }

  bool IsBoolean() const override {
    return std::is_same<T, bool>::value;
  }

  bool Parse(const std::string &str) override {
    std::istringstream iss(str);
    iss >> std::noskipws >> *value_;
    return iss.eof();
  }

  const std::string& Name() const override {
    return name_;
  }

 protected:
  const std::string name_;
  const std::string help_;
  T *value_;
};


// Specialization for bool type
template <>
std::string Option<bool>::Help(size_t w) const {
  std::ostringstream oss;
  oss << std::setw(w) << std::left << name_ << " : " << help_
      << " (type = " << TypeInfo<bool>::Name()
      << ", value = \"" << (*value_ ? "true" : "false") << "\")";
  return oss.str();
}

template <>
bool Option<bool>::Parse(const std::string& str) {
  if (str == "" || str == "true" || str == "1") {
    *value_ = true;
    return true;
  } else if (str == "false" || str == "0") {
    *value_ = false;
    return true;
  } else {
    return false;
  }
}

// Specialization for std::string
template <>
bool Option<std::string>::Parse(const std::string &str) {
  std::istringstream iss(str);
  return std::getline(iss, *value_, '\0').eof();
}

}  // namespace cmd
}  // namespace kws_eval

#endif // CMD_OPTION_H
