#ifndef READER_PLAINTEXTREADER_H_
#define READER_PLAINTEXTREADER_H_

#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

#include "reader/PlainTextMapEventReader.h"
#include "mapper/IdentityMapper.h"

namespace kws {
namespace reader {

using kws::mapper::IdentityMapper;

template<typename E>
class PlainTextReader : public PlainTextMapEventReader<IdentityMapper<E>> {
 public:
  typedef PlainTextMapEventReader<IdentityMapper<E>> BaseType;
  PlainTextReader() : BaseType(&mapper) {}

 protected:
  IdentityMapper<E> mapper;
};

}  // namespace reader
}  // namespace kws

#endif  // READER_PLAINTEXTREADER_H_
