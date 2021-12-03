#include <array>
#include <cassert>
#include <string>
#include <vector>

#include "aoc_util.h"
#include "fmt/core.h"

const int NUM_DIGITS = 12;

int process_report(std::vector<int> report, const bool selector) {
  int i = NUM_DIGITS - 1;

  while (report.size() > 1) {
    int counter = 0;

    for (const auto &num : report) {
      if (num & (1 << i)) {
        counter++;
      } else {
        counter--;
      }
    }

    const bool most_common = counter >= 0;
    std::vector<int> report_new;

    for (const auto &num : report) {
      const bool report_bit = (num >> i) & 1;
      if (selector) {
        if (most_common == report_bit) {
          report_new.emplace_back(num);
        }
      } else if (most_common != report_bit) {
        report_new.emplace_back(num);
      }
    }
    report = report_new;
    i--;
  }
  return report[0];
}

int main(int argc, char *argv[]) {
  assert(argc == 2);

  const auto diag_report = process_input(argv[1], [](const std::string &x) {
    return (int)strtoull(x.c_str(), NULL, 2);
  });

  // Part 1
  int gamma = 0;
  int epsilon;
  {
    MeasureTime m("Part 1");
    std::array<int, NUM_DIGITS> counters{0};

    for (const auto &num : diag_report) {
      for (int i = 0; i < NUM_DIGITS; i++) {
        if (num & (1 << i)) {
          counters[i]++;
        } else {
          counters[i]--;
        }
      }
    }

    for (int i = 0; i < NUM_DIGITS; i++) {
      if (counters[i] > 0) {
        gamma |= (1 << i);
      }
    }

    epsilon = gamma ^ 0xfff;
  }
  fmt::print("Part 1: {}\n", gamma * epsilon);

  // Part 2
  int oxy_gen;
  int co2_scr;
  {
    MeasureTime m("Part 2");
    oxy_gen = process_report(diag_report, 1);
    co2_scr = process_report(diag_report, 0);
  }
  fmt::print("Part 2: {}\n", oxy_gen * co2_scr);
}
