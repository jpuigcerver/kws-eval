#ifndef READER_PLAINTEXTMAPEVENTREADER_H_
#define READER_PLAINTEXTMAPEVENTREADER_H_

#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

#include "reader/Reader.h"

namespace kws {
namespace reader {

template <typename Mapper>
class PlainTextMapEventReader : public Reader<typename Mapper::OutputType> {
 public:
  typedef typename Mapper::InputType E1;
  typedef typename Mapper::OutputType E2;

  explicit PlainTextMapEventReader(Mapper& mapper) : mapper_(&mapper) {}

  explicit PlainTextMapEventReader(Mapper* mapper) : mapper_(mapper) {}

  bool Read(std::istream* is, std::vector<E2>* events) const override {
    events->clear();
    std::string buff;
    for (size_t n = 1; std::getline(*is, buff); ++n) {
      std::istringstream iss(buff);
      // Skip whitespaces
      iss >> std::ws;
      if (iss.peek() == '#') continue;   // Comment line
      if (iss.eof()) continue;           // Empty line
      // Read event, and consume all whitespaces at the end of the line.
      E1 event;
      iss >> event;
      if (!iss.eof()) iss >> std::ws;
      if (iss.fail() || !iss.eof()) {
        std::cerr << "ERROR: Failed to read event from line " << n << std::endl;
        return false;
      }
      events->push_back((*mapper_)(event));
    }
    return is->eof();
  }

  bool Read(const std::string &filepath, std::vector<E2> *events) const override {
    std::ifstream fs(filepath, std::ios_base::in);
    if (!fs.is_open()) return false;
    const bool r = Read(&fs, events);
    fs.close();
    return r;
  }

 protected:
  Mapper* mapper_;
};

}  // namespace reader
}  // namespace kws

#endif  // READER_PLAINTEXTMAPEVENTREADER_H_
