#ifndef READER_PLAINTEXTREADER_H_
#define READER_PLAINTEXTREADER_H_

#include <algorithm>
#include <vector>

namespace kws {
namespace reader {

template <typename E>
class PlainTextReader {
 public:
  bool Read(std::istream* is, std::vector<E>* events) const {
    events->clear();
    E event;
    while(*is >> event) {
      std::cerr << event << std::endl;
      events->push_back(event);
    }
    std::sort(events->begin(), events->end(), std::greater<E>());
    return is->eof();
  }

  bool Read(const std::string& filename, std::vector<E>* events) const {
    std::ifstream fs(filename, std::ios_base::in);
    if (!fs.is_open()) return false;
    const bool r = Read(&fs, events);
    fs.close();
    return r;
  }
};

}  // namespace reader
}  // namespace kws

#endif  // READER_PLAINTEXTREADER_H_
