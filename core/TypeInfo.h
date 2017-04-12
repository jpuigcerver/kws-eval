#ifndef CORE_TYPEINFO_H_
#define CORE_TYPEINFO_H_

#include <cstdint>
#include <string>
#include <typeinfo>

namespace kws {
namespace core {

template <typename T>
class TypeInfo {
 public:
  static const std::string& Name() {
    static std::string name = typeid(T).name();
    return name;
  }
};

template <>
const std::string& TypeInfo<bool>::Name() {
  static std::string name = "bool";
  return name;
}

template <>
const std::string& TypeInfo<int8_t>::Name() {
  static std::string name = "int8_t";
  return name;
}

template <>
const std::string& TypeInfo<int16_t>::Name() {
  static std::string name = "int16_t";
  return name;
}

template <>
const std::string& TypeInfo<int32_t>::Name() {
  static std::string name = "int32_t";
  return name;
}

template <>
const std::string& TypeInfo<int64_t>::Name() {
  static std::string name = "int64_t";
  return name;
}

template <>
const std::string& TypeInfo<uint8_t>::Name() {
  static std::string name = "uint8_t";
  return name;
}

template <>
const std::string& TypeInfo<uint16_t>::Name() {
  static std::string name = "uint16_t";
  return name;
}

template <>
const std::string& TypeInfo<uint32_t>::Name() {
  static std::string name = "uint32_t";
  return name;
}

template <>
const std::string& TypeInfo<uint64_t>::Name() {
  static std::string name = "uint64_t";
  return name;
}

template <>
const std::string& TypeInfo<std::string>::Name() {
  static std::string name = "string";
  return name;
}

}  // namespace core
}  // namespace kws

#endif // CORE_TYPEINFO_H_
