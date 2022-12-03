#include "aoc_includes.h"

typedef std::uint64_t chunk_t;
typedef std::uint64_t literal_t;

constexpr int BITS_IN_CHUNK = sizeof(chunk_t) * 8;
constexpr int VER_LEN = 3;
constexpr int TYPE_ID_LEN = 3;
constexpr int LIT_CHUNK_LEN = 5;  // includes 1-bit header
constexpr int LEN_TYPE_ID_LEN = 1;
constexpr int TOT_LEN_BITS_LEN = 15;
constexpr int NUM_SUBPKT_LEN = 11;

enum class LengthTypeId { TotalLengthBits, NumberSubpackets };

enum class TypeId {
  Sum,
  Product,
  Minimum,
  Maximum,
  Literal,
  GreaterThan,
  LessThan,
  EqualTo
};

struct Header {
  std::uint8_t version;
  TypeId type_id;
};

struct Packet {
  Header header;
  literal_t literal_value;
  std::vector<Packet> packets;
};

class RawData {
 public:
  std::vector<chunk_t> data;
  Packet parse_packet();

 private:
  Header get_header();
  literal_t get_literal_value();
  LengthTypeId get_length_type_id();
  std::uint32_t get_total_length_bits();
  std::uint32_t get_number_subpackets();
  std::uint32_t pos = 0;

  chunk_t bitrange(const int bit_length);
  static inline chunk_t bitmask(const int mask) {
    if (mask == BITS_IN_CHUNK) {
      return std::numeric_limits<chunk_t>::max();
    }
    return ((chunk_t)1 << mask) - 1;
  }
};

chunk_t RawData::bitrange(const int bit_length) {
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

Header RawData::get_header() {
  std::uint8_t version = bitrange(VER_LEN);
  TypeId type_id = (TypeId)bitrange(TYPE_ID_LEN);
  return Header{version, type_id};
}

LengthTypeId RawData::get_length_type_id() {
  return bitrange(LEN_TYPE_ID_LEN) ? LengthTypeId::NumberSubpackets
                                   : LengthTypeId::TotalLengthBits;
}

std::uint32_t RawData::get_total_length_bits() {
  return bitrange(TOT_LEN_BITS_LEN);
}

std::uint32_t RawData::get_number_subpackets() {
  return bitrange(NUM_SUBPKT_LEN);
}

Packet RawData::parse_packet() {
  const auto header = get_header();
  auto current_packet = Packet{header};
  if (header.type_id == TypeId::Literal) {
    current_packet.literal_value = get_literal_value();
  } else {
    if (get_length_type_id() == LengthTypeId::TotalLengthBits) {
      const auto total_length_bits = get_total_length_bits();
      const auto start_pos = pos;
      while (pos < (start_pos + total_length_bits)) {
        current_packet.packets.emplace_back(parse_packet());
      }
    } else {  // LengthTypeId::NumberSubpackets
      const auto number_subpackets = get_number_subpackets();
      for (std::uint32_t i = 0; i < number_subpackets; i++) {
        current_packet.packets.emplace_back(parse_packet());
      }
    }
  }
  return current_packet;
}

literal_t RawData::get_literal_value() {
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

std::uint32_t get_version_sum(const Packet &packet_in) {
  std::uint32_t sum = packet_in.header.version;
  for (const auto &packet : packet_in.packets) {
    sum += get_version_sum(packet);
  }
  return sum;
}

std::uint64_t get_packet_value(const Packet &packet_in) {
  std::uint64_t value = 0;
  switch (packet_in.header.type_id) {
    case TypeId::Sum:
      value = 0;
      for (const auto &packet : packet_in.packets) {
        value += get_packet_value(packet);
      }
      break;
    case TypeId::Product:
      value = 1;
      for (const auto &packet : packet_in.packets) {
        value *= get_packet_value(packet);
      }
      break;
    case TypeId::Minimum:
      value = std::numeric_limits<decltype(value)>::max();
      for (const auto &packet : packet_in.packets) {
        if (const auto temp = get_packet_value(packet); temp < value) {
          value = temp;
        }
      }
      break;
    case TypeId::Maximum:
      value = 0;
      for (const auto &packet : packet_in.packets) {
        if (const auto temp = get_packet_value(packet); temp > value) {
          value = temp;
        }
      }
      break;
    case TypeId::Literal:
      value = packet_in.literal_value;
      break;
    case TypeId::GreaterThan:
      value = get_packet_value(packet_in.packets[0]) >
                      get_packet_value(packet_in.packets[1])
                  ? 1
                  : 0;
      break;
    case TypeId::LessThan:
      value = get_packet_value(packet_in.packets[0]) <
                      get_packet_value(packet_in.packets[1])
                  ? 1
                  : 0;
      break;
    case TypeId::EqualTo:
      value = get_packet_value(packet_in.packets[0]) ==
                      get_packet_value(packet_in.packets[1])
                  ? 1
                  : 0;
      break;
  }
  return value;
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

  {
    MeasureTime m{"Total"};
    const auto outer_packet = raw_data.parse_packet();
    fmt::print("Part 1: {}\n", get_version_sum(outer_packet));
    fmt::print("Part 2: {}\n", get_packet_value(outer_packet));
  }
}
