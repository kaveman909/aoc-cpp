#include "aoc_includes.h"

struct Packet {
  std::variant<int, std::vector<Packet>> pkt;
  Packet(const int val) { pkt = val; }
  Packet() { pkt = std::vector<Packet>(); }
  void emplace_back(int i) {
    assert(pkt.index() == 1);
    std::get<1>(pkt).emplace_back(i);
  }
  Packet *emplace_back() {
    assert(pkt.index() == 1);
    std::get<1>(pkt).emplace_back();
    return &std::get<1>(pkt).back();
  }
  std::vector<Packet> &get() {
    assert(pkt.index() == 1);
    return std::get<1>(pkt);
  }
};

void aoc(char *f) {
  Packet p;
  p.emplace_back(5);
  p.emplace_back();
  for (auto &x : p.get()) {
    fmt::print("{}\n", x.pkt.index());
    if (x.pkt.index() == 0) {
      fmt::print("  {}\n", std::get<int>(x.pkt));
    }
  }
}
