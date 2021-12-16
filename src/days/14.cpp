#include "aoc_includes.h"

auto local_process_input(char *fpath) {
  std::unordered_map<std::string, std::string> pair_rules;

  std::ifstream in(fpath);
  char buf[MAX_LEN];
  in.getline(buf, MAX_LEN);  // first line is polymer template
  std::string poly_template{buf};
  in.getline(buf, MAX_LEN);  // empty line

  while (in.getline(buf, MAX_LEN)) {
    std::string s{buf};
    std::vector<std::string> parsed;
    boost::split(parsed, s, boost::is_any_of(" ->"),
                 boost::algorithm::token_compress_on);
    pair_rules[parsed[0]] = parsed[1];
  }
  return std::pair{poly_template, pair_rules};
}

void aoc(char *f) {
  auto [polymer, pair_rules] = local_process_input(f);

  for (int i = 0; i < 10; i++) {
    // Calculate new inserts
    std::string new_inserts;
    for (size_t i = 0; i < polymer.length() - 1; i++) {
      new_inserts += pair_rules[polymer.substr(i, 2)];
    }
    // Create new string with interleaved data
    std::string combined;
    for (auto &&[p, n] : iter::zip_longest(polymer, new_inserts)) {
      if (p) combined += *p;
      if (n) combined += *n;
    }
    polymer = combined;
  }

  std::sort(polymer.begin(), polymer.end());
  std::string poly_unique;
  std::unique_copy(polymer.begin(), polymer.end(),
                   std::back_inserter(poly_unique));
  std::vector<int> poly_counts;
  for (const auto &el : poly_unique) {
    poly_counts.emplace_back(std::count(polymer.begin(), polymer.end(), el));
  }
  std::sort(poly_counts.begin(), poly_counts.end());
  fmt::print("Part 1: {}\n", *(poly_counts.end() - 1) - *poly_counts.begin());
}
