
#pragma once
#include <vector>
#include <array>
#include <cstdint>

#define DOL_MAX_SECTION_COUNT 18

#define DOLHDR_OFFSETS_OFF 0x0
#define DOLHDR_OFFSET_SIZE 0x4
#define DOLHDR_ADDRESSES_OFF 0x48
#define DOLHDR_ADDRESS_SIZE 0x4
#define DOLHDR_SIZES_OFF 0x90
#define DOLHDR_SIZE_SIZE 0x4
#define DOLHDR_BSSADDR_OFF 0xd8
#define DOLHDR_BSSLEN_OFF 0xdc
#define DOLHDR_ENTRYPOINT_OFF 0xe0

class DolHeaderRaw {
public:
    std::array<uint32_t, DOL_MAX_SECTION_COUNT> offsets;
    std::array<uint32_t, DOL_MAX_SECTION_COUNT> addresses;
    std::array<uint32_t, DOL_MAX_SECTION_COUNT> lengths;

    uint32_t bss_address;
    uint32_t bss_length;
    uint32_t entry_point;

    DolHeaderRaw(uint8_t* dol);
    std::ostream& print(std::ostream& os) const;
};

/**
 * @brief Represents a valid, non-bss DOL section
 */
class DolSection {
public:
    uint32_t offset;
    uint32_t address;
    uint32_t length;
    uint8_t* data = nullptr;

    DolSection(uint8_t* dol, uint32_t offset, uint32_t address, uint32_t length);
    ~DolSection();
    DolSection(DolSection&& other) noexcept : offset(other.offset), address(other.address),
        length(other.length), data(other.data) {
        other.data = nullptr;
    }
    std::ostream& print(std::ostream& os) const;
};

class Dol {
public:
    // Raw data of DOL file
    DolHeaderRaw hdr;

    // More manageable representation of DOL data
    std::vector<DolSection> secs;

    Dol(uint8_t* dol);
    std::ostream& print(std::ostream& os) const;
};