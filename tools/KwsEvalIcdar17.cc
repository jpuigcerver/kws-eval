#include <iostream>
#include <fstream>

#include "core/Event.h"
#include "core/DocumentBoundingBox.h"
#include "core/ScoredEvent.h"
#include "evaluator/SimpleEvaluator.h"
#include "reader/PlainTextReader.h"
#include "scorer/IntersectionOverHypothesisAreaScorer.h"

using kws::core::DocumentBoundingBox;
using kws::core::Event;
using kws::core::ScoredEvent;
using kws::evaluator::SimpleEvaluator;
using kws::reader::PlainTextReader;
using kws::scorer::IntersectionOverHypothesisAreaScorer;

int main(int argc, char** argv) {
  typedef Event<DocumentBoundingBox<uint16_t>> RefEvent;
  typedef ScoredEvent<DocumentBoundingBox<uint16_t>> HypEvent;
  typedef IntersectionOverHypothesisAreaScorer<RefEvent,HypEvent> OverlapScorer;

  if (argc != 2 && argc != 3) {
    std::cerr << "Usage: " << argv[0] << " reference hypothesis" << std::endl;
    return 1;
  }

  // Read reference events
  std::vector<RefEvent> ref_events;
  PlainTextReader<RefEvent> ref_reader;
  if (!ref_reader.Read(argv[1], &ref_events)) {
    std::cerr << "ERROR: Failed reading file \"" << argv[1] << "\"!"
              << std::endl;
    return 1;
  }

  // Read hypothesis events
  std::vector<HypEvent> hyp_events;
  PlainTextReader<HypEvent> hyp_reader;
  if (!(argc == 2
        ? hyp_reader.Read(&std::cin, &hyp_events)
        : hyp_reader.Read(argv[2], &hyp_events))) {
    if (argc == 2) {
      std::cerr << "ERROR: Failed reading from stdin!" << std::endl;
    } else {
      std::cerr << "ERROR: Failed reading file \"" << argv[2] << "\"!"
                << std::endl;
    }
    return 1;
  }


  //IntersectionOverHypothesisAreaScorer<RefEvent, HypEvent> scorer(0.5);

  SimpleEvaluator<RefEvent, HypEvent, OverlapScorer> eval(OverlapScorer(0.5));


  return 0;
}
