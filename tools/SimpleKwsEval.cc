#include "core/ScoredEvent.h"
#include "matcher/SimpleMatcher.h"
#include "reader/PlainTextReader.h"
#include "scorer/TrivialScorer.h"
#include "tools/GenericKwsEvalTool.h"

using kws::core::Event;
using kws::core::ScoredEvent;
using kws::matcher::SimpleMatcher;
using kws::reader::PlainTextReader;
using kws::scorer::TrivialScorer;
using kws::tools::GenericKwsEvalTool;

int main(int argc, const char** argv) {
  const std::string description =
      "  Perform simple evaluation for segmentation-based keyword spotting.";

  typedef Event<std::string, std::string> RefEvent;
  typedef ScoredEvent<RefEvent> HypEvent;

  PlainTextReader<RefEvent> ref_reader;
  PlainTextReader<HypEvent> hyp_reader;

  TrivialScorer<RefEvent, HypEvent> scorer;
  SimpleMatcher<RefEvent, HypEvent> matcher(&scorer);

  GenericKwsEvalTool<PlainTextReader<RefEvent>,PlainTextReader<HypEvent>,
      SimpleMatcher<RefEvent, HypEvent>> tool(&ref_reader, &hyp_reader,
                                              &matcher, description);

  return tool.Main(argc, argv);
}
