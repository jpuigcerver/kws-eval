#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include <map>
#include <string>
#include <vector>

#include "cmd/Argument.h"
#include "cmd/Option.h"

namespace kws {
namespace cmd {

class Parser {
 public:
  explicit Parser(const std::string& prog) : prog_(prog), desc_("") {}

  Parser(const std::string& prog, const std::string& description) :
    prog_(prog), desc_(description) {}

  ~Parser() {
    for (auto kw : opts_) {
      delete kw.second;
    }
    for (auto a : args_) {
      delete a;
    }
    for (auto a : opt_args_) {
      delete a;
    }
  }

  std::string Help() const {
    std::ostringstream oss;
    // Show program name
    oss << "Usage: " << prog_;
    // Show options. Note: Only shows the first 5 options, and then [ ... ]
    {
      int i = 0; auto it = opts_.begin();
      for (; it != opts_.end() && i < 5; ++it, ++i) {
        oss << " [ " << it->second->Name() << " ]";
      }
      if (it != opts_.end()) {
        oss << " [ ... ]";
      }
    }
    // Show mandatory arguments
    for (auto a : args_) {
      oss << " " << a->Name();
    }
    // Show optional arguments
    {
      for (auto a : opt_args_) {
        oss << " [ " << a->Name();
      }
      for (size_t i = 0; i < opt_args_.size(); ++i) {
        oss << " ]";
      }
    }
    oss << std::endl;

    // Show description, if available.
    if (!desc_.empty()) {
      oss << std::endl;
      oss << "Description:" << std::endl;
      oss << desc_ << std::endl;
    }

    // Determine the maximum width of the names.
    size_t max_w = 0;
    for (auto o : opts_) { max_w = std::max(max_w, o.second->Name().size()); }
    for (auto a : args_) { max_w = std::max(max_w, a->Name().size()); }
    for (auto a : opt_args_) { max_w = std::max(max_w, a->Name().size()); }


    if (!args_.empty() || !opt_args_.empty()) {
      oss << std::endl;
      oss << "Arguments:" << std::endl;
      for (const auto& a : args_) {
        oss << "  " << a->Help(max_w)  << std::endl;
      }
      for (const auto& a : opt_args_) {
        oss << "  " << a->Help(max_w)  << std::endl;
      }
    }

    if (!opts_.empty()) {
      oss << std::endl;
      oss << "Options:" << std::endl;
      for (const auto& o : opts_) {
        oss << "  " << o.second->Help(max_w)  << std::endl;
      }
    }

    return oss.str();
  }

  bool Parse(const int argc, const char** argv) {
    bool show_help = false;

    int i = 1; // skip argv[0]
    // Parse OPTIONS
    while (i < argc) {
      const std::string arg = argv[i];
      if (arg.length() < 2 || arg[0] != '-') break;
      ++i;   // Options have values, point to the next argument.
      if (arg == "--") { break; }   // Skip options section with "--".
      if (arg == "--help") { show_help = true; continue; }
      const std::string opt_name = arg.substr(2);
      auto opt_it = opts_.find(opt_name);
      if (opt_it == opts_.end()) {
        std::cerr << "ERROR: Unknown option '" << arg << "'" << std::endl;
        return false;
      }
      bool ok = false;
      if (opt_it->second->IsBoolean()) {
        ok = opt_it->second->Parse(i < argc && argv[i][0] != '-'
                                   ? argv[i++] : "");
      } else {
        ok = i < argc && opt_it->second->Parse(argv[i++]);
      }
      if (!ok) {
        std::cerr << "ERROR: Invalid value for option '"
                  << arg << "'" << std::endl;
        return false;
      }
    }
    // If --help was used, just exit.
    if (show_help) {
      std::cerr << Help() << std::endl;
      exit(0);
    }
    // Parse mandatory ARGUMENTS
    size_t j = 0;
    for (j = 0; i < argc && j < args_.size(); ++i, ++j) {
      if (!args_[j]->Parse(argv[i])) {
        std::cerr << "ERROR: Invalid value for " << j + 1
                  << "positional argument" << std::endl;
        return false;
      }
    }
    // Check that all the mandatory positional cmd were given.
    if (j < args_.size()) {
      std::cerr << "ERROR: Missing " << args_.size() - j << " arguments."
                << std::endl;
      return false;
    }
    // Parse optional ARGUMENTS
    size_t k = 0;
    for (k = 0; i < argc && k < opt_args_.size(); ++i, ++k) {
      if (!opt_args_[k]->Parse(argv[i])) {
        std::cerr << "ERROR: Invalid value for " << j + k + 1
                  << " positional argument" << std::endl;
        return false;
      }
    }

    // Check that no extra positional cmd were given.
    if (i < argc) {
      const size_t max_args = args_.size() + opt_args_.size();
      const size_t given_args = max_args + (argc - i);
      std::cerr << "ERROR: Expecting at most " << max_args
                << " positional cmd, but " << given_args << " were given"
                << std::endl;
      return false;
    }

    return true;
  }

  template <typename T>
  bool RegisterOption(const std::string& name, const std::string& help,
                      T* value) {
    if (name == "help") return false;  // reserved option name
    auto opt = new Option<T>("--" + name, help, value);
    if (!opts_.emplace(name, opt).second) {
      delete opt;
      return false;
    }
    return true;
  }

  template <typename T>
  void RegisterArgument(const std::string& name, const std::string& help,
                        T* value) {
    args_.emplace_back(new Argument<T>(name, help, value));
  }

  template <typename T>
  void RegisterOptArgument(const std::string& name, const std::string& help,
                           T* value) {
    opt_args_.emplace_back(new Argument<T>(name, help, value));
  }

 private:
  const std::string prog_;
  const std::string desc_;
  std::map<std::string, BaseOption*> opts_;
  std::vector<BaseOption*> args_;
  std::vector<BaseOption*> opt_args_;
};

}  // namespace cmd
}  // namespace kws

#endif // CMD_PARSER_H
