#include "aoc_includes.h"

bool find_in(const std::string &small, const std::string &large) {
  size_t sum = 0;
  for (const auto c : small) {
    if (large.find(c) != std::string::npos) {
      sum++;
    }
  }
  return sum == small.size();
}

struct PatternMap {
  std::array<std::string, 10> digits;
  std::array<std::string, 4> output;

  PatternMap(std::vector<std::string> in) {
    assert(in.size() == 14);
    for (size_t i = 0; i < digits.size(); i++) {
      digits[i] = in[i];
    }
    for (size_t i = 0; i < output.size(); i++) {
      output[i] = in[i + 10];
    }
  }
};

auto decode(std::array<std::string, 10> digits) {
  std::map<std::string, int> out;

  // get 1, 7, 4, and 8 out of the way, and can use the info for others
  out[digits[0]] = 1;
  out[digits[1]] = 7;
  out[digits[2]] = 4;
  out[digits[9]] = 8;
  // deduce len 6:
  // 9 has same as 4
  // else, 0 has same as 1
  // leftover is 6
  std::string six;  // used in later search so convenience var
  for (int i = 6; i <= 8; i++) {
    if (find_in(digits[2], digits[i])) {
      out[digits[i]] = 9;
    } else if (find_in(digits[0], digits[i])) {
      out[digits[i]] = 0;
    } else {
      out[digits[i]] = 6;
      six = digits[i];
    }
  }
  // deduce len 5:
  // 3 has same as 1
  // find 5 within 6
  // leftover is 2
  for (int i = 3; i <= 5; i++) {
    if (find_in(digits[0], digits[i])) {
      out[digits[i]] = 3;
    } else if (find_in(digits[i], six)) {
      out[digits[i]] = 5;
    } else {
      out[digits[i]] = 2;
    }
  }

  return out;
}

void aoc(char *f) {
  const auto in = process_input(f, [](const std::string s) {
    // e.g. ceg gedcfb ec eabfdg gcdabe baged cabgf gbaec fecagdb eacd | efcgbad
    // adfecbg gec abgce
    std::vector<std::string> parsed;
    boost::split(parsed, s, boost::is_any_of(" |"),
                 boost::algorithm::token_compress_on);

    std::sort(
        parsed.begin(), parsed.begin() + 10,
        [](std::string &s1, std::string &s2) { return s1.size() < s2.size(); });
    std::for_each(parsed.begin(), parsed.end(),
                  [](std::string &s) { std::sort(s.begin(), s.end()); });

    return PatternMap{parsed};
  });

  int sum = 0;
  int sum2 = 0;
  for (const auto &pattern : in) {
    auto decoded = decode(pattern.digits);
    int mult = 1000;

    for (auto &output : pattern.output) {
      const std::array<int, 4> easy_vals = {1, 4, 7, 8};
      if (std::find(easy_vals.begin(), easy_vals.end(), decoded[output]) !=
          easy_vals.end()) {
        sum += 1;
      }
      sum2 += decoded[output] * mult;
      mult /= 10;
    }
  }
  fmt::print("Part 1: {}\n", sum);
  fmt::print("Part 2: {}\n", sum2);
}
