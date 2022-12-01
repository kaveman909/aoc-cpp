#include "aoc_includes.h"
using namespace std;

typedef map<char, int> Location;

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
      beacons.back().emplace_back(Location{{'x', x}, {'y', y}, {'z', z}});
    }  // else, blank line, do nothing
  }

  return beacons;
}

auto mode_with_index(vector<Location> &in) {
  int idx = 0, idxw = -1, len = 1, lenw = 1;

  for (size_t i = 1; i < in.size(); i++) {
    if (in[i]['d'] == in[i - 1]['d']) {
      if (idxw == -1) {
        idxw = i - 1;
      }
      lenw++;
    } else {
      if (lenw > len) {
        len = lenw;
        idx = idxw;
      }
      // reset
      lenw = 1;
      idxw = -1;
    }
  }
  return tuple{idx, len};
}

void aoc(char *f) {
  const vector base_faces = {"xyz", "yzx", "zxy"};
  const vector<array<int, 2>> rotation_dirs = {{1, 2}, {2, 1}, {1, 2}, {2, 1}};
  const vector<array<int, 2>> rotation_signs = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};

  auto beacons = local_process_input(f);
  const MeasureTime m{"Total"};

  for (size_t iref = 0; iref < beacons.size() - 1; iref++) {
    auto &scanner_ref = beacons[iref];

    for (size_t icomp = iref + 1; icomp < beacons.size(); icomp++) {
      auto &scanner_comp = beacons[icomp];

      for (int pos = 1; pos >= 0; pos--) {
        for (const auto &base_face : base_faces) {
          for (int rot = 0; rot < (int)rotation_dirs.size(); rot++) {
          // for (const auto rot : rotation_dirs) {
            const auto x = base_face[0];
            int yi, zi, xs, ys, zs;

            if (pos) {
              yi = rotation_dirs[rot][0];
              zi = rotation_dirs[rot][1];

              xs = 1;
              ys = rotation_dirs[rot][0];
              zs = rotation_dirs[rot][1];
            } else {
              yi = rotation_dirs[rot][1];
              zi = rotation_dirs[rot][0];

              xs = -1;
              ys = rotation_dirs[rot][1];
              zs = rotation_dirs[rot][0];
            }
            const auto y = base_face[yi];
            const auto z = base_face[zi];

            vector<Location> scanner_comp_mod;
            for (auto &beacon : scanner_comp) {
              scanner_comp_mod.emplace_back(Location{{'x', beacon[x] * xs},
                                                     {'y', beacon[y] * ys},
                                                     {'z', beacon[z] * zs}});
            }
            vector<Location> p2p;
            for (auto &ref : scanner_ref) {
              for (auto &comp : scanner_comp_mod) {
                int dist = 0;
                p2p.emplace_back(Location{});
                for (auto l : {'x', 'y', 'z'}) {
                  auto diff = ref[l] - comp[l];
                  dist += diff * diff;
                  p2p.back()[l] = diff;
                }
                p2p.back()['d'] = dist;
              }
            }
            sort(p2p.begin(), p2p.end());
            auto [idx, len] = mode_with_index(p2p);
            if (len > 1) {
              fmt::print("idx: {}, len: {}\n", idx, len);
            }
            if (len >= 12) {
              // may need to add in check if all x,y,z diffs are equal.
              NOOP();
            }
          }
        }
      }
    }
  }
}
