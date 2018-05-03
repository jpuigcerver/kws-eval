#include "core/DocumentBoundingBox.h"
#include "core/DocumentBoundingBoxEventSet.h"
#include "core/ScoredEvent.h"
#include "core/ShapedEvent.h"
#include "matcher/SimpleMatcher.h"
#include "reader/PlainTextReader.h"
#include "scorer/IntersectionOverHypothesisAreaScorer.h"
#include "tools/GenericKwsEvalTool.h"

using kws::core::DocumentBoundingBox;
using kws::core::ShapedEvent;
using kws::core::ScoredEvent;
using kws::matcher::SimpleMatcher;
using kws::reader::PlainTextReader;
using kws::scorer::IntersectionOverHypothesisAreaScorer;
using kws::tools::GenericKwsEvalTool;
using kws::mapper::IdentityMapper;

int main(int argc, const char **argv) {
  const std::string description =
      "  Official evaluation tool for ICDAR2017 H-KWS Competition.";

  typedef ShapedEvent<std::string, DocumentBoundingBox<uint32_t>> RefEvent;
  typedef ScoredEvent<RefEvent> HypEvent;
  typedef PlainTextReader<RefEvent> RefReader;
  typedef PlainTextReader<HypEvent> HypReader;
  typedef SimpleMatcher<RefEvent, HypEvent> Matcher;
  typedef IdentityMapper<std::string> StrMapper;

  RefReader ref_reader;
  HypReader hyp_reader;

  IntersectionOverHypothesisAreaScorer<RefEvent, HypEvent> scorer(0.5);
  Matcher matcher(&scorer);

  StrMapper query_mapper;
  GenericKwsEvalTool<RefReader, HypReader, Matcher, StrMapper> tool(
      &ref_reader, &hyp_reader, &matcher, &query_mapper, description);

  return tool.Main(argc, argv);
}
