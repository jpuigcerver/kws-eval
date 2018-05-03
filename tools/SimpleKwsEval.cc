#include "core/ScoredEvent.h"
#include "matcher/SimpleMatcher.h"
#include "reader/PlainTextMapEventReader.h"
#include "scorer/TrivialScorer.h"
#include "tools/GenericKwsEvalTool.h"
#include "mapper/StringEventToIntMapper.h"
#include "mapper/ScoredEventMapper.h"

using kws::core::Event;
using kws::core::ScoredEvent;
using kws::matcher::SimpleMatcher;
using kws::reader::PlainTextMapEventReader;
using kws::scorer::TrivialScorer;
using kws::tools::GenericKwsEvalTool;

int main(int argc, const char **argv) {
  const std::string description =
      "  Perform simple evaluation for segmentation-based keyword spotting.";

  typedef Event<int32_t, int32_t> RefEvent;
  typedef ScoredEvent<RefEvent> HypEvent;

  typedef kws::mapper::StringToIntMapper<int32_t> StrMapper;
  typedef kws::mapper::StringEventToIntMapper<int32_t> RefMapper;
  typedef kws::mapper::ScoredEventMapper<RefMapper> HypMapper;

  typedef PlainTextMapEventReader<RefMapper> RefReader;
  typedef PlainTextMapEventReader<HypMapper> HypReader;
  typedef SimpleMatcher<RefEvent, HypEvent> Matcher;

  StrMapper str2int_mapper;
  RefMapper ref_mapper(&str2int_mapper);
  HypMapper hyp_mapper(&ref_mapper);

  RefReader ref_reader(&ref_mapper);
  HypReader hyp_reader(&hyp_mapper);

  TrivialScorer<RefEvent, HypEvent> scorer;
  Matcher matcher(&scorer);

  GenericKwsEvalTool<RefReader, HypReader, Matcher, StrMapper> tool(
      &ref_reader,
      &hyp_reader,
      &matcher,
      &str2int_mapper,
      description);
  return tool.Main(argc, argv);
}
