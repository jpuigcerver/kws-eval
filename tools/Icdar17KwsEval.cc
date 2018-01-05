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

int main(int argc, const char** argv) {
  const std::string description =
      "  Official evaluation tool for ICDAR2017 H-KWS Competition.";

  typedef ShapedEvent<std::string, DocumentBoundingBox<uint32_t>> RefEvent;
  typedef ScoredEvent<RefEvent> HypEvent;

  PlainTextReader<RefEvent> ref_reader;
  PlainTextReader<HypEvent> hyp_reader;

  IntersectionOverHypothesisAreaScorer<RefEvent, HypEvent> scorer(0.5);
  SimpleMatcher<RefEvent, HypEvent> matcher(&scorer);

  GenericKwsEvalTool<PlainTextReader<RefEvent>,PlainTextReader<HypEvent>,
      SimpleMatcher<RefEvent, HypEvent>> tool(&ref_reader, &hyp_reader,
                                              &matcher, description);

  return tool.Main(argc, argv);
}
