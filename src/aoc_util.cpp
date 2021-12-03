#include "aoc_util.h"
#include <chrono>
#include <iostream>
#include <string>

using namespace std::chrono;

std::vector<std::string> process_input(char *fpath) {
  std::ifstream in(fpath);
  std::vector<std::string> v;

  for (char buf[MAX_LEN]; in.getline(buf, MAX_LEN);) {
    v.emplace_back(buf);
  }

  return v;
}

MeasureTime::MeasureTime() {
  start = high_resolution_clock::now();
}

MeasureTime::MeasureTime(std::string str) : label{str} {
  start = high_resolution_clock::now();
}

MeasureTime::~MeasureTime() {
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  std::cout << "Duration(" << label << "): " << duration.count() << " microseconds" << '\n';
}
