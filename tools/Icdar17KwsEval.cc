#include <iostream>
#include <fstream>
#include <unordered_map>

#ifdef WITH_GLOG
#include <glog/logging.h>
#endif

#include "cmd/Parser.h"
#include "core/Assessment.h"
#include "core/Event.h"
#include "core/DocumentBoundingBox.h"
#include "core/ScoredEvent.h"
#include "matcher/SimpleMatcher.h"
#include "reader/PlainTextReader.h"
#include "scorer/ThresholdBoundingBoxScorer.h"

using kws::cmd::Parser;
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

template <typename E, typename C>
void filter_events(const C& queryset, std::vector<E>* events) {
  std::vector<E> events_aux = *events;
  events->clear();
  for (const auto& e : events_aux) {
    if (queryset.count(e.Query()) > 0) events->push_back(e);
  }
}

int main(int argc, const char** argv) {
#ifdef WITH_GLOG
  google::InitGoogleLogging(argv[0]);
#endif

  std::string ref_filename = "";
  std::string hyp_filename = "";
  std::string queryset_filename = "";
  std::string querygroups_filename = "";
  std::string matches_filename = "";
  bool interpolated_precision = true;
  bool trapezoid_integral = true;

  Parser cmd_parser(
      argv[0],
      "  Official evaluation tool for ICDAR2017 H-KWS Competition");
  // Options
  cmd_parser.RegisterOption(
      "interpolated_precision",
      "Use interpolated precision.",
      &interpolated_precision);
  cmd_parser.RegisterOption(
      "trapezoid_integral", "Use trapezoid integral to compute AP/mAP.",
      &trapezoid_integral);
  cmd_parser.RegisterOption(
      "query_set",
      "File containing the set of queries to consider. Events regarding other "
      "queries are excluded from both references and hypotheses.",
      &queryset_filename);
  cmd_parser.RegisterOption(
      "dump_matches",
      "Dump the raw matches to this file.",
      &matches_filename);
  cmd_parser.RegisterOption(
      "query_groups",
      "File containing the set of query groups to consider in the mAP. "
      "This option supersedes the queries read from the file --query_set.",
      &querygroups_filename);
  // Arguments
  cmd_parser.RegisterArgument(
      "references",
      "File containing the reference events (ground-truth).",
      &ref_filename);
  cmd_parser.RegisterOptArgument(
      "hypotheses",
      "File containing the hypothesis events (submission).",
      &hyp_filename);
  // Parse command line.
  if (!cmd_parser.Parse(argc, argv)) {
    std::cerr << std::endl << cmd_parser.Help() << std::endl;
    return 1;
  }

  // Read reference events
  if (ref_filename.empty()) {
    std::cerr << "ERROR: Empty filename was given for the references!"
              << std::endl;
    return 1;
  }
  std::vector<RefEvent> ref_events;
  PlainTextReader<RefEvent> ref_reader;
  if (!ref_reader.Read(ref_filename, &ref_events)) {
    std::cerr << "ERROR: Failed reading file \"" << ref_filename << "\"!"
              << std::endl;
    return 1;
  }
  std::cerr << "INFO: Number of reference events read = " << ref_events.size()
            << std::endl;

  // Read hypothesis events
  std::vector<HypEvent> hyp_events;
  PlainTextReader<HypEvent> hyp_reader;
  if (!(hyp_filename.empty()
        ? hyp_reader.Read(&std::cin, &hyp_events)
        : hyp_reader.Read(hyp_filename, &hyp_events))) {
    if (argc == 2) {
      std::cerr << "ERROR: Failed reading from stdin!" << std::endl;
    } else {
      std::cerr << "ERROR: Failed reading file \"" << hyp_filename << "\"!"
                << std::endl;
    }
    return 1;
  }
  std::cerr << "INFO: Number of hypothesis events read = " << hyp_events.size()
            << std::endl;

  std::map<std::string, std::string> query2group;
  std::map<std::string, std::vector<std::string>> group2query;

  // If a queryset filename was given, filter out events from queries not in
  // this file.
  if (!queryset_filename.empty() && querygroups_filename.empty()) {
    std::ifstream qfs(queryset_filename, std::ios_base::in);
    if (!qfs.is_open()) {
      std::cerr << "ERROR: Query set file \"" << queryset_filename
                << "\" could not be read!" << std::endl;
      return 1;
    }
    std::string q;
    while(qfs >> q) { query2group[q] = q; }
    qfs.close();
    std::cerr << "INFO: " << query2group.size() << " queries were read from \""
              << queryset_filename << "\"" << std::endl;
  } else if (!querygroups_filename.empty()) {
    std::ifstream qfs(querygroups_filename, std::ios_base::in);
    if (!qfs.is_open()) {
      std::cerr << "ERROR: Query groups file \"" << querygroups_filename
                << "\" could not be read!" << std::endl;
      return 1;
    }
    std::string line, s;
    while (std::getline(qfs, line)) {
      std::vector<std::string> fields;
      std::istringstream iss(line);
      while (iss >> s) { fields.push_back(s); }
      if (fields.size() < 2) {
        std::cerr << "ERROR: Query groups file \"" << querygroups_filename
                  << "\" has a wrong format!" << std::endl;
        return 1;
      }
      for (size_t i = 1; i < fields.size(); ++i) {
        query2group[fields[i]] = fields[0];
      }
    }
    qfs.close();
    std::cerr << "INFO: " << query2group.size() << " queries were read from \""
              << querygroups_filename << "\"" << std::endl;
  }

  if (!query2group.empty()) {
    // Filter out reference events.
    const size_t num_ref_events = ref_events.size();
    filter_events(query2group, &ref_events);
    if (num_ref_events != ref_events.size()) {
      std::cerr << "INFO: Number of kept reference events = "
                << ref_events.size() << std::endl;
    }
    // Filter out hypothesis events.
    const size_t num_hyp_events = hyp_events.size();
    filter_events(query2group, &hyp_events);
    if (num_hyp_events != hyp_events.size()) {
      std::cerr << "INFO: Number of kept hypothesis events = "
                << hyp_events.size() << std::endl;
    }
    // Add values to the group2query map
    for (const auto& kv : query2group) {
      group2query.emplace(kv.second, std::vector<std::string>())
          .first->second.push_back(kv.first);
    }
    if (!querygroups_filename.empty()) {
      std::cerr << "INFO: " << group2query.size() << " groups were read "
                << "from \"" << querygroups_filename << "\"" << std::endl;
    }
  }

  // Sort hypotheses in decreasing order of their score.
  std::sort(hyp_events.begin(), hyp_events.end(), std::greater<HypEvent>());

  // Match hypothesis events against the references.
  IntersectionOverHypothesisAreaScorer<RefEvent, HypEvent> scorer(0.5);
  SimpleMatcher<RefEvent, HypEvent> matcher(&scorer);
  const auto matches = matcher.Match(ref_events, hyp_events);

  // Optionally, dump raw matches to the given file.
  if (!matches_filename.empty()) {
    std::ofstream mfs(matches_filename, std::ios_base::out);
    if (!mfs.is_open()) {
      std::cerr << "ERROR: Dump matches file \"" << matches_filename
                << "\" could not be opened for write!" << std::endl;
      return 1;
    }
    for (const auto& m : matches) {
      mfs << m << std::endl;
    }
    mfs << "#### REPEATED MATCHES ####" << std::endl;
    for (const auto& m : matcher.GetRepeatedMatches()) {
      mfs << "## " << m << std::endl;
    }
    mfs.close();
  }

  {
    // Count total hits + false positives.
    size_t nh = 0;
    for (const auto& m : matches) { nh += m.GetError().NH(); }
    if (nh < hyp_events.size()) {
      std::cerr << "INFO: Effective number of hypotheses is " << nh << ". "
                << "The rest of hypotheses were considered repetitions "
                << "of some other match, and ignored." << std::endl;
    } else if (nh > hyp_events.size()) {
      std::cerr << "ERROR: Effective number of hypotheses IS GREATER "
                << "than the original number! This should not happen ever, "
                << "contact organizers." << std::endl;
      return 1;
    }
  }

  // Compute GLOBAL Average Precision
  {
    // Collapse all events with the same score into a single point in the
    // precision/recall curve.
    const auto collapsed_errors = CollapseMatches(matches);
    // Compute precision and recall curves
    std::vector<float> pr_curve, rc_curve;
    ComputePrecisionAndRecall(collapsed_errors,
                              interpolated_precision,
                              &pr_curve, &rc_curve);
    // Compute & print Average Precision
    std::cout << "gAP = "
              << ComputeAP(pr_curve, rc_curve, trapezoid_integral)
              << std::endl;
  }
  // Compute MEAN Average Precision
  {
    // First, split the matches according to their query/group ID
    std::unordered_map<std::string, std::list<Match<RefEvent, HypEvent>>>
        query_to_matches;
    for (const auto& m : matches) {
      const std::string& query = m.HasRef()
          ? m.GetRef().Query()
          : m.GetHyp().Query();
      query_to_matches.emplace(
              (query2group.count(query) ? query2group[query] : query),
              std::list<Match<RefEvent, HypEvent>>())
          .first->second.push_back(m);
    }
    // Sum the AP of each individual query
    float sumAP = 0.0f;
    for (const auto& qm : query_to_matches) {
      const auto collapsed_errors = CollapseMatches(qm.second);
      std::vector<float> pr_curve, rc_curve;
      ComputePrecisionAndRecall(collapsed_errors,
                                interpolated_precision,
                                &pr_curve, &rc_curve);
      const float aux = ComputeAP(pr_curve, rc_curve, trapezoid_integral);
      sumAP += aux;
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
