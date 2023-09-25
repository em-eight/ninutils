
#pragma once

#include <cstring>
#include <cstdint>
#include <iomanip>
#include <bit>

#define DEC_FMT(X) std::noshowbase << std::dec << X
#define HEX_FMT(X) std::showbase << std::hex << X
#define SHEX_FMT(X) std::showbase << std::hex << (X>=0 ? "" : "-") << (X>=0 ? X : -X) // signed format
#define BOOL_FMT(X) (0==X ? "false" : "true")
#define WIDTH(X,n) std::left << std::setw(n) << std::setfill(' ') << X
#define DEC_FMTW(X,n) std::noshowbase << std::dec << std::left << std::setw(n) << std::setfill(' ') << X
#define HEX_FMTW(X,n) std::showbase << std::hex << std::left << std::setw(n) << std::setfill(' ') << X
#define BOOL_FMTW(X,n) std::left << std::setw(n) << std::setfill(' ') << (0==X ? "false" : "true")
#define NUM(X) +X // hack that prints char/uint8_t as int

namespace ninutils {
inline uint8_t be8(uint8_t x) { return x; }

inline uint16_t be16(uint16_t x) {
  if constexpr (std::endian::native != std::endian::big) return ((x & 0xff) >> 8) | (x << 8);
  return x;
}

inline uint32_t be32(uint32_t num) {
  if constexpr (std::endian::native != std::endian::big) return ((num & 0xff000000) >> 24) | ((num & 0x00ff0000) >> 8) | ((num & 0x0000ff00) << 8) | (num << 24);
  return num;
}

inline uint8_t readbe8(void* src) {
    uint8_t num;
    memcpy(&num, src, 1);
    return num;
}

inline uint16_t readbe16(void* src) {
    uint16_t num;
    memcpy(&num, src, 2);
    return be16(num);
}

inline uint32_t readbe32(void* src) {
    uint32_t num;
    memcpy(&num, src, 4);
    return be32(num);
}
} // ns ninutils
