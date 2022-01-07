#include "aoc_includes.h"

typedef std::uint64_t chunk_t;
typedef std::uint32_t literal_t;

constexpr int BITS_IN_CHUNK = sizeof(chunk_t) * 8;
constexpr int VER_LEN = 3;
constexpr int TYPE_ID_LEN = 3;
constexpr int LIT_CHUNK_LEN = 5;  // includes 1-bit header

enum LengthTypeId { TotalLengthBits, NumberSubpackets };

enum TypeId { Literal = 4 };

struct Header {
  std::uint32_t version : VER_LEN;
  std::uint32_t type_id : TYPE_ID_LEN;
};

struct Packet {
  Header header;
  // This could technically be a union since we are either a literal value, or
  // contain sub-packets; however, this causes all of the special member
  // functions to be non-trivial and it was taking way too long to try to
  // implement all of them for very little benefit.
  literal_t literal_value;
  std::vector<Packet> packets;
};

class RawData {
 public:
  std::vector<chunk_t> data;
  chunk_t bitrange(const int bit_length);
  auto get_header();
  auto get_literal_value();
  int pos = 0;

 private:
  static inline chunk_t bitmask(const int mask) {
    if (mask == BITS_IN_CHUNK) {
      return std::numeric_limits<chunk_t>::max();
    }
    return ((chunk_t)1 << mask) - 1;
  }
};

auto RawData::get_literal_value() {
  literal_t literal_value = 0;
  while (true) {
    const auto chunk = bitrange(LIT_CHUNK_LEN);
    literal_value |= chunk & bitmask(LIT_CHUNK_LEN - 1);
    if (!(chunk & (1 << (LIT_CHUNK_LEN - 1)))) {
      break;
    }
    literal_value <<= (LIT_CHUNK_LEN - 1);
  }
  return literal_value;
}

auto RawData::get_header() {
  std::uint32_t version = bitrange(VER_LEN);
  std::uint32_t type_id = bitrange(TYPE_ID_LEN);
  return Header{version, type_id};
}

[[nodiscard]] chunk_t RawData::bitrange(const int bit_length) {
  if (bit_length == 0) {
    return 0;
  }
  const int bit_offset = pos;
  pos += bit_length;

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

  // Split up top-level packet into chunks for processing
  constexpr int HEX_IN_CHUNK = sizeof(chunk_t) * 2;
  RawData raw_data;
  for (size_t i = 0; i < in.size(); i += HEX_IN_CHUNK) {
    auto [result, udata] =
        scn::scan_tuple<chunk_t>(in.substr(i, HEX_IN_CHUNK), "{:x}");
    raw_data.data.emplace_back(udata);
  }

  auto header = raw_data.get_header();
  auto outer = Packet{header};
  if (header.type_id == TypeId::Literal) {
    outer.literal_value = raw_data.get_literal_value();
  }
  NOOP();
}
