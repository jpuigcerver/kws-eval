#include <iostream>
#include <fstream>
#include <unordered_map>

#include "core/Assessment.h"
#include "core/Event.h"
#include "core/DocumentBoundingBox.h"
#include "core/ScoredEvent.h"
#include "matcher/SimpleMatcher.h"
#include "reader/PlainTextReader.h"
#include "scorer/IntersectionOverHypothesisAreaScorer.h"

using kws::core::CollapseMatches;
using kws::core::ComputeAP;
using kws::core::ComputePrecisionAndRecall;
using kws::core::DocumentBoundingBox;
using kws::core::Event;
using kws::core::Match;
using kws::core::ScoredEvent;
using kws::matcher::SimpleMatcher;
using kws::reader::PlainTextReader;
using kws::scorer::IntersectionOverHypothesisAreaScorer;

typedef Event<std::string, DocumentBoundingBox<uint32_t>> RefEvent;
typedef ScoredEvent<std::string, DocumentBoundingBox<uint32_t>> HypEvent;

int main(int argc, char** argv) {
  if (argc != 2 && argc != 3) {
    std::cerr << "Usage: " << argv[0] << " references [hypotheses]"
              << std::endl;
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

  // Sort hypotheses in decreasing order of their score.
  std::sort(hyp_events.begin(), hyp_events.end(), std::greater<HypEvent>());

  // Match hypothesis events against the references.
  IntersectionOverHypothesisAreaScorer<RefEvent, HypEvent> scorer(0.5);
  SimpleMatcher<RefEvent, HypEvent> matcher(&scorer);
  const auto matches = matcher.Match(ref_events, hyp_events);

  // Compute GLOBAL Average Precision
  {
    // Collapse all events with the same score into a single point in the
    // precision/recall curve.
    const auto collapsed_errors = CollapseMatches(matches);
    // Compute precision and recall curves
    std::vector<float> pr_curve, rc_curve;
    ComputePrecisionAndRecall(collapsed_errors,
                              true /* interpolate precision */,
                              &pr_curve, &rc_curve);
    // Compute & print Average Precision
    std::cout << "gAP = "
              << ComputeAP(pr_curve, rc_curve, true /* trapezoid integral */)
              << std::endl;
  }
  // Compute MEAN Average Precision
  {
    // First, split the matches according to their query ID
    std::unordered_map<std::string, std::list<Match<RefEvent, HypEvent>>>
        query_to_matches;
    for (const auto& m : matches) {
      const std::string& query = m.HasRef()
          ? m.GetRef().Query()
          : m.GetHyp().Query();
      query_to_matches.emplace(query, std::list<Match<RefEvent, HypEvent>>())
          .first->second.push_back(m);
    }
    // Sum the AP of each individual query
    float sumAP = 0.0f;
    for (const auto& qm : query_to_matches) {
      const auto collapsed_errors = CollapseMatches(qm.second);
      std::vector<float> pr_curve, rc_curve;
      ComputePrecisionAndRecall(collapsed_errors,
                                true /* interpolate precision */,
                                &pr_curve, &rc_curve);
      sumAP += ComputeAP(pr_curve, rc_curve, true /* trapezoid integral */);
    }
    // Print MEAN Average Precision
    std::cout << "mAP = "
              << (query_to_matches.size() > 0
                  ? sumAP / query_to_matches.size()
                  : 0.0f)
              << std::endl;
  }

  return 0;
}
