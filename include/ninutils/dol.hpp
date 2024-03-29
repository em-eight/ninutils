
#pragma once
#include <vector>
#include <array>
#include <cstdint>
#include <optional>

#include "extra_info.hpp"

#define DOL_MAX_TEXT_SECTION_COUNT 7
#define DOL_MAX_DATA_SECTION_COUNT 11
#define DOL_MAX_SECTION_COUNT 18


namespace ninutils {

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

    /**
     * @brief Whether it is a text section or data section
     */
    bool isText;
    std::string name = "";

    DolSection(uint32_t offset, uint32_t address, uint32_t length, bool text, std::string name = "");
    inline bool isBss() const { return offset == 0; }
    std::ostream& print(std::ostream& os) const;
};

class Dol {
public:
    // Raw data of DOL file
    DolHeaderRaw hdr;

    // More manageable representation of DOL data
    std::vector<DolSection> secs;
    
    // file data
    uint8_t* file = nullptr;
    size_t fileSize;

    Dol(uint8_t* dol, size_t fileSize, std::optional<ExtraInfo> extra_info = std::nullopt);
    ~Dol();
    std::optional<uint32_t> getSectionIdxByName(const std::string& name);
    std::optional<uint32_t> getSectionIdxContainingAddress(uint32_t vma);
    std::ostream& print(std::ostream& os) const;
private:
    void setSectionName(uint8_t sec, std::optional<ExtraInfo> extra_info);
};
} // ns ninutils