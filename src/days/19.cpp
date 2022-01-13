#include "aoc_includes.h"
using namespace std;

struct Location {
  int x;
  int y;
  int z;
};

static auto local_process_input(char *fpath) {
  vector<vector<Location>> beacons;
  std::ifstream in(fpath);
  char buf[MAX_LEN];

  while (in.getline(buf, MAX_LEN)) {
    // e.g. "--- scanner 0 ---"
    if (const auto [result, id] =
            scn::scan_tuple<int>(buf, "--- scanner {} ---");
        result) {
      (void)id;  // unused
      beacons.emplace_back(vector<Location>{});
    }
    // e.g. "654,-533,-962"
    else if (const auto [result, x, y, z] =
                 scn::scan_tuple<int, int, int>(buf, "{},{},{}");
             result) {
      beacons.back().emplace_back(Location{x, y, z});
    }  // else, blank line, do nothing
  }

  return beacons;
}

void aoc(char *f) { const auto beacons = local_process_input(f); }
