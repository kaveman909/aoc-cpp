#pragma once

#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

static const int MAX_LEN = 150;

std::vector<std::string> process_input(char *fpath);

template <typename F,
          typename T = std::invoke_result<F, const std::string &>::type>
auto process_input(char *fpath, F conv_f) {
  std::ifstream in(fpath);
  std::vector<T> v;

  for (char buf[MAX_LEN]; in.getline(buf, MAX_LEN);) {
    const std::string s{buf};
    v.emplace_back(conv_f(s));
  }

  return v;
}
