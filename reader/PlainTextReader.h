#ifndef READER_PLAINTEXTREADER_H_
#define READER_PLAINTEXTREADER_H_

#include <algorithm>
#include <fstream>
#include <vector>

namespace kws {
namespace reader {

template <typename E>
class PlainTextReader {
 public:
  bool Read(std::istream* is, std::vector<E>* events) const {
    events->clear();
    std::string buff;
    for (size_t n = 1; std::getline(*is, buff); ++n) {
      std::istringstream iss(buff);
      // Skip whitespaces 
      iss >> std::ws;
      if (iss.peek() == '#') continue;   // Comment line
      if (iss.eof()) continue;           // Empty line
      // Read event, and consume all whitespaces at the end of the line.
      E event;
      iss >> event >> std::ws;
      if (iss.fail() || !iss.eof()) {
        std::cerr << "ERROR: Failed to read event from line " << n << std::endl;
        return false;
      }
      events->push_back(event);
    }
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
