#include "aoc_includes.h"

class Section {
 public:
  int begin;
  int end;

  bool contains(const Section &s) const {
    return (begin <= s.begin && end >= s.end) ? true : false;
  }

  bool overlaps(const Section &s) const {
    return (end < s.begin || s.end < begin) ? false : true;
  }
};

class Pair {
 public:
  Section s_left;
  Section s_right;

  bool fully_contains() const {
    return s_left.contains(s_right) || s_right.contains(s_left);
  }

  bool overlaps() const { return s_left.overlaps(s_right); }
};

static auto local_process_input(char *fpath) {
  std::vector<Pair> pairs;
  std::ifstream in(fpath);
  char buf[MAX_LEN];

  while (in.getline(buf, MAX_LEN)) {
    const auto [result, lbegin, lend, rbegin, rend] =
        scn::scan_tuple<int, int, int, int>(buf, "{}-{},{}-{}");
    assert(result);
    pairs.emplace_back(Pair{Section{lbegin, lend}, Section{rbegin, rend}});
  }

  return pairs;
}

void aoc(char *f) {
  const auto pairs = local_process_input(f);
  int contains;
  {
    MeasureTime m{"Part 1"};
    contains = std::count_if(pairs.begin(), pairs.end(),
                             [](const Pair &p) { return p.fully_contains(); });
  }
  fmt::print("Part 1: {}\n", contains);

  int overlaps;
  {
    MeasureTime m{"Part 2"};
    overlaps = std::count_if(pairs.begin(), pairs.end(),
                             [](const Pair &p) { return p.overlaps(); });
  }
  fmt::print("Part 2: {}\n", overlaps);
}
