#ifndef TOOLS_GENERICKWSEVALTOOL_H_
#define TOOLS_GENERICKWSEVALTOOL_H_

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef WITH_GLOG
#include <glog/logging.h>
#endif

#include "cmd/Parser.h"
#include "core/Assessment.h"
#include "core/Bootstrapping.h"

namespace kws {
namespace tools {

using kws::cmd::Parser;
using kws::core::CollapseMatches;
using kws::core::ComputeAP;
using kws::core::ComputePercentileBootstrapCI;
using kws::core::ComputePrecisionAndRecall;
using kws::core::Match;

template<typename E, typename C>
void filter_events(const C &queryset, std::vector<E> *events) {
  std::vector<E> events_aux = *events;
  events->clear();
  for (const auto &e : events_aux) {
    if (queryset.count(e.Query()) > 0) events->push_back(e);
  }
}

template<class RefReader, class HypReader, class Matcher>
class GenericKwsEvalTool {
 public:
  typedef typename Matcher::RefEvent RefEvent;
  typedef typename Matcher::HypEvent HypEvent;

  GenericKwsEvalTool(RefReader *ref_reader, HypReader *hyp_reader,
                     Matcher *matcher, const std::string &description = "") :
      ref_reader_(ref_reader), hyp_reader_(hyp_reader), matcher_(matcher),
      description_(description) {}

  int Main(int argc, const char **argv) {
#ifdef WITH_GLOG
    google::InitGoogleLogging(argv[0]);
#endif

    std::string ref_filename = "";
    std::string hyp_filename = "";
    std::string queryset_filename = "";
    std::string querygroups_filename = "";
    std::string matches_filename = "";
    bool collapse_matches = true;
    bool interpolated_precision = true;
    bool trapezoid_integral = true;
    bool bootstrap_ci_gap = false;
    bool bootstrap_ci_map = false;
    size_t bootstrap_samples = 10000;
    size_t bootstrap_seed = 0x12345;
    double bootstrap_alpha = 0.05;

    // Options
    Parser cmd_parser(argv[0], description_);
    cmd_parser.RegisterOption(
        "collapse_matches",
        "Collapse all matches with the same score before computing precision "
            "and recall curves.",
        &collapse_matches);
    cmd_parser.RegisterOption(
        "interpolated_precision",
        "Use interpolated precision.",
        &interpolated_precision);
    cmd_parser.RegisterOption(
        "trapezoid_integral", "Use trapezoid integral to compute AP/mAP.",
        &trapezoid_integral);
    cmd_parser.RegisterOption(
        "query_set",
        "File containing the set of queries to consider. Events regarding "
            "other queries are excluded from both references and hypotheses.",
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
    cmd_parser.RegisterOption(
        "bootstrap_ci_gap",
        "Compute bootstrapped confidence intervals for the Global AP.",
        &bootstrap_ci_gap);
    cmd_parser.RegisterOption(
        "bootstrap_ci_map",
        "Compute bootstrapped confidence intervals for the Mean AP.",
        &bootstrap_ci_map);
    cmd_parser.RegisterOption(
        "bootstrap_samples",
        "Use this number of bootstrapped samples to compute the confidence "
            "intervals.",
        &bootstrap_samples);
    cmd_parser.RegisterOption(
        "bootstrap_seed",
        "Use this random seed to generate the bootstrapped samples.",
        &bootstrap_seed);
    cmd_parser.RegisterOption(
        "bootstrap_alpha",
        "Use this alpha value to compute confidence intervals.",
        &bootstrap_alpha);
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
    if (!ref_reader_->Read(ref_filename, &ref_events)) {
      std::cerr << "ERROR: Failed reading file \"" << ref_filename << "\"!"
                << std::endl;
      return 1;
    }
    std::cerr << "INFO: Number of reference events read = " << ref_events.size()
              << std::endl;

    // Read hypothesis events
    std::vector<HypEvent> hyp_events;
    if (!(hyp_filename.empty()
          ? hyp_reader_->Read(&std::cin, &hyp_events)
          : hyp_reader_->Read(hyp_filename, &hyp_events))) {
      if (argc == 2) {
        std::cerr << "ERROR: Failed reading from stdin!" << std::endl;
      } else {
        std::cerr << "ERROR: Failed reading file \"" << hyp_filename << "\"!"
                  << std::endl;
      }
      return 1;
    }
    std::cerr << "INFO: Number of hypothesis events read = "
              << hyp_events.size()
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
      while (qfs >> q) { query2group[q] = q; }
      qfs.close();
      std::cerr << "INFO: " << query2group.size()
                << " queries were read from \""
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
      std::cerr << "INFO: " << query2group.size()
                << " queries were read from \""
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
      for (const auto &kv : query2group) {
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
    const auto matches = matcher_->Match(ref_events, hyp_events);

    // Optionally, dump raw matches to the given file.
    if (!matches_filename.empty()) {
      std::ofstream mfs(matches_filename, std::ios_base::out);
      if (!mfs.is_open()) {
        std::cerr << "ERROR: Dump matches file \"" << matches_filename
                  << "\" could not be opened for write!" << std::endl;
        return 1;
      }
      for (const auto &m : matches) {
        mfs << m << std::endl;
      }
      mfs << "#### REPEATED MATCHES ####" << std::endl;
      for (const auto &m : matcher_->GetRepeatedMatches()) {
        mfs << "## " << m << std::endl;
      }
      mfs.close();
    }

    {
      // Count total hits + false positives.
      size_t nh = 0;
      for (const auto &m : matches) { nh += m.GetError().NH(); }
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

    // Group matches by query/group.
    typedef std::vector<std::vector<Match<RefEvent, HypEvent>>>
        VectorOfVectorsOfMatches;
    VectorOfVectorsOfMatches matches_by_group;
    core::GroupMatchesByQueryGroup(matches, query2group, &matches_by_group);

    // Compute GLOBAL Average Precision
    if (bootstrap_ci_gap) {
      auto gap_statistic =
          [collapse_matches, interpolated_precision, trapezoid_integral](
              const VectorOfVectorsOfMatches &sampled) -> double {
            return core::ComputeGlobalAP(sampled,
                                         collapse_matches,
                                         interpolated_precision,
                                         trapezoid_integral);
          };
      core::MatchesByQuerySampler<RefEvent, HypEvent> sampler(bootstrap_seed);
      double lower_bound = 0.0, upper_bound = 0.0;
      const double global_ap = core::ComputePercentileBootstrapCI(
          matches_by_group, bootstrap_samples, bootstrap_alpha, gap_statistic,
          &sampler, &lower_bound, &upper_bound);
      std::cout << "gAP = " << global_ap
                << " [" << lower_bound << ", " << upper_bound << "]"
                << std::endl;
    } else {
      std::cout << "gAP = "
                << core::ComputeGlobalAP(matches,
                                         collapse_matches,
                                         interpolated_precision,
                                         trapezoid_integral)
                << std::endl;
    }

    // Compute MEAN Average Precision
    if (bootstrap_ci_map) {
      auto map_statistic =
          [collapse_matches, interpolated_precision, trapezoid_integral](
              const VectorOfVectorsOfMatches &sampled) -> double {
            return core::ComputeMeanAP(sampled,
                                       collapse_matches,
                                       interpolated_precision,
                                       trapezoid_integral);
          };
      core::MatchesByQuerySampler<RefEvent, HypEvent> sampler(bootstrap_seed);
      double lower_bound = 0.0, upper_bound = 0.0;
      const double mean_ap = core::ComputePercentileBootstrapCI(
          matches_by_group, bootstrap_samples, bootstrap_alpha, map_statistic,
          &sampler, &lower_bound, &upper_bound);
      std::cout << "mAP = " << mean_ap
                << " [" << lower_bound << ", " << upper_bound << "]"
                << std::endl;
    } else {
      std::cout << "mAP = "
                << core::ComputeMeanAP(matches_by_group,
                                       collapse_matches,
                                       interpolated_precision,
                                       trapezoid_integral)
                << std::endl;
    }

    return 0;
  }

 protected:
  RefReader *ref_reader_;
  HypReader *hyp_reader_;
  Matcher *matcher_;
  std::string description_;
};

}  // namespace tools
}  // namespace kws

#endif  // TOOLS_GENERICKWSEVALTOOL_H_
