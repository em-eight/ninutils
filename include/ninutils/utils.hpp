
#pragma once

#include <cstring>
#include <cstdint>
#include <iomanip>

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

inline uint8_t readbe8(void* src) {
    uint8_t num;
    memcpy(&num, src, 1);
    return num;
}

inline uint16_t readbe16(void* src) {
    uint16_t num;
    memcpy(&num, src, 2);
    return __builtin_bswap16(num);
}

inline uint32_t readbe32(void* src) {
    uint32_t num;
    memcpy(&num, src, 4);
    return __builtin_bswap32(num);
}

inline uint64_t readbe64(void* src) {
    uint64_t num;
    memcpy(&num, src, 8);
    return __builtin_bswap64(num);
}
} // ns ninutils