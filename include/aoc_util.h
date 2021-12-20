#pragma once

#include <chrono>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>
#include <compare>

static const int MAX_LEN = 5000;

std::vector<std::string> process_input(char *fpath);

// clang requires the 'typename' prior to the std::invoke_result, whereas
// gcc does not.  Seems to be a lack in the clang C++20 implementation.
template <typename F, typename T = typename std::invoke_result<
                          F, const std::string &>::type>
auto process_input(char *fpath, F conv_f) {
  std::ifstream in(fpath);
  std::vector<T> v;

  for (char buf[MAX_LEN]; in.getline(buf, MAX_LEN);) {
    const std::string s{buf};
    v.emplace_back(conv_f(s));
  }

  return v;
}

class MeasureTime {
 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::string label;

 public:
  MeasureTime();
  MeasureTime(std::string str);
  ~MeasureTime();
};

namespace advent {
struct Point {
  int x;
  int y;
  friend auto operator<=>(Point const &lhs, Point const &rhs) = default;
};
}  // namespace advent
