#include "aoc_util.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std::chrono;

std::vector<std::string> process_input(char *fpath) {
  std::ifstream in(fpath);
  std::vector<std::string> v;

  for (char buf[MAX_LEN]; in.getline(buf, MAX_LEN);) {
    v.emplace_back(buf);
  }

  return v;
}

MeasureTime::MeasureTime() { start = high_resolution_clock::now(); }

MeasureTime::MeasureTime(std::string str) : label{str} {
  start = high_resolution_clock::now();
}

MeasureTime::~MeasureTime() {
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  std::cout << "Duration(" << label << "): " << duration.count()
            << " microseconds" << '\n';
}

// namespace advent {
// bool operator==(const Point &a, const Point &b) {
//   return (a.x == b.x) && (a.y == b.y);
// }

// bool operator<(const Point &a, const Point &b) {
//   if (a.x < b.x) return true;
//   if (a.x > b.x) return false;
//   if (a.y < b.y) return true;
//   if (a.y > b.y) return false;
//   return false;
// }
// }  // namespace advent
