#ifndef READER_READER_H_
#define READER_READER_H_

#include <fstream>
#include <vector>

namespace kws {
namespace reader {

template <typename E>
class Reader {
 public:
  virtual
  bool Read(std::istream* is, std::vector<E>* events) const = 0;

  virtual
  bool Read(const std::string& filepath, std::vector<E>* events) const = 0;
};

}  // namespace reader
}  // namespace kws

#endif  // READER_READER_H_