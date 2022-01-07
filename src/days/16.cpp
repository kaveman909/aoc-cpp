#include "aoc_includes.h"

typedef std::uint64_t chunk_t;
constexpr int BITS_IN_CHUNK = sizeof(chunk_t) * 8;

enum class LengthTypeId { TotalLengthBits, NumberSubpackets };

enum class TypeId { Literal = 4 };

struct Packet {
  std::uint8_t version : 3;
  std::uint8_t type_id : 3;
  union {
    std::uint64_t literal_value;
    std::vector<Packet> packets;
  };
};

class RawData {
 public:
  std::vector<chunk_t> data;
  chunk_t bitrange(const int bit_offset, const int bit_length);

 private:
  static inline chunk_t bitmask(const int mask) {
    if (mask == BITS_IN_CHUNK) {
      return std::numeric_limits<chunk_t>::max();
    }
    return ((chunk_t)1 << mask) - 1;
  }
};

[[nodiscard]] chunk_t RawData::bitrange(const int bit_offset,
                                        const int bit_length) {
  if (bit_length == 0) {
    return 0;
  }
  const int byte_base = bit_offset / BITS_IN_CHUNK;
  const int bit_base = bit_offset % BITS_IN_CHUNK;

  if ((bit_base + bit_length) <= BITS_IN_CHUNK) {
    // all in one chunk
    return (data[byte_base] >> (BITS_IN_CHUNK - bit_length - bit_offset)) &
           bitmask(bit_length);
  }
  // assume most is 2 chunks
  const int bit_length0 = BITS_IN_CHUNK - bit_base;
  const int bit_length1 = bit_length - bit_length0;
  const chunk_t chunk0 =
      (data[byte_base] >> (BITS_IN_CHUNK - bit_length0 - bit_offset)) &
      bitmask(bit_length0);
  const chunk_t chunk1 =
      (data[byte_base + 1] >> (BITS_IN_CHUNK - bit_length1)) &
      bitmask(bit_length1);
  return (chunk0 << bit_length1) | chunk1;
}

void aoc(char *f) {
  scn::owning_file file{f, "r"};
  auto [result, in] = scn::scan_tuple<std::string>(file, "{}");

  // Split up top-level packet into 8-byte chunks for processing
  constexpr int HEX_IN_CHUNK = sizeof(chunk_t) * 2;
  RawData raw_data;
  for (size_t i = 0; i < in.size(); i += HEX_IN_CHUNK) {
    auto [result, udata] =
        scn::scan_tuple<chunk_t>(in.substr(i, HEX_IN_CHUNK), "{:x}");
    raw_data.data.emplace_back(udata);
  }
}
