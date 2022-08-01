
#pragma once

#include <endian.h>
#include <cstring>
#include <cstdint>

#define DEC_FMT(X) std::dec << X
#define HEX_FMT(X) "0x" << std::hex << X
#define BOOL_FMT(X) (0==X ? "false" : "true")
#define NUM(X) +X // hack that prints char/uint8_t as int

uint8_t readbe8(void* src) {
    uint8_t num;
    memcpy(&num, src, 1);
    return num;
}

uint16_t readbe16(void* src) {
    uint16_t num;
    memcpy(&num, src, 2);
    return htobe16(num);
}

uint32_t readbe32(void* src) {
    uint32_t num;
    memcpy(&num, src, 4);
    return htobe32(num);
}

uint64_t readbe64(void* src) {
    uint64_t num;
    memcpy(&num, src, 8);
    return htobe64(num);
}
