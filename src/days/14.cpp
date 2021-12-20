#include "aoc_includes.h"

long count_max_min_singles(const std::string &polymer,
                           std::unordered_map<std::string, long> &pair_counters,
                           std::unordered_map<char, long> single_counters) {
  for (auto &count : pair_counters) {
    for (int i = 0; i < 2; i++) {
      single_counters[count.first[i]] += count.second;
    }
  }
  std::for_each(single_counters.begin(), single_counters.end(),
                [](auto &count) { count.second /= 2; });
  // compensate for first/last element in polymer not being double-counted
  single_counters[polymer.front()]++;
  single_counters[polymer.back()]++;

  auto [min, max] = std::minmax_element(
      single_counters.begin(), single_counters.end(),
      [](const auto &p1, const auto &p2) { return p1.second < p2.second; });

  return max->second - min->second;
}

auto local_process_input(char *fpath) {
  std::unordered_map<std::string, std::pair<std::string, std::string>>
      pair_rules;

  std::ifstream in(fpath);
  char buf[MAX_LEN];
  std::unordered_map<std::string, long> pair_counters;
  std::unordered_map<char, long> single_counters;

  in.getline(buf, MAX_LEN);  // first line is polymer template
  std::string poly_template{buf};
  in.getline(buf, MAX_LEN);  // empty line

  while (in.getline(buf, MAX_LEN)) {
    std::string s{buf};
    std::vector<std::string> parsed;
    boost::split(parsed, s, boost::is_any_of(" ->"),
                 boost::algorithm::token_compress_on);
    pair_rules[parsed[0]].first = parsed[0][0] + parsed[1];
    pair_rules[parsed[0]].second = parsed[1] + parsed[0][1];
    pair_counters[parsed[0]] = 0;
    single_counters[parsed[1][0]] = 0;
  }
  return std::tuple{poly_template, pair_rules, pair_counters, single_counters};
}

void aoc(char *f) {
  auto [polymer, pair_rules, pair_counters, single_counters] =
      local_process_input(f);

  // (1) Get initial counts
  for (size_t i = 0; i < polymer.length() - 1; i++) {
    pair_counters[polymer.substr(i, 2)]++;
  }

  // (2) Run the Simulation
  {
    MeasureTime m{"Sim Time"};
    for (int i = 0; i < 40; i++) {
      if (i == 10) {
        fmt::print("Part 1: {}\n", count_max_min_singles(polymer, pair_counters,
                                                         single_counters));
      }
      auto pair_counters_cp = pair_counters;
      for (const auto &rule : pair_rules) {
        if (pair_counters_cp[rule.first] > 0) {
          long count = pair_counters_cp[rule.first];
          const auto &[new_a, new_b] = rule.second;
          pair_counters[rule.first] -= count;
          pair_counters[new_a] += count;
          pair_counters[new_b] += count;
        }
      }
    }
  }

  fmt::print("Part 2: {}\n",
             count_max_min_singles(polymer, pair_counters, single_counters));
}
