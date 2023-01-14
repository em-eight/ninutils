
#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <optional>

#include "extra_info.hpp"

#define R_PPC_NONE 0
#define R_PPC_ADDR32 1
#define R_PPC_ADDR24 2
#define R_PPC_ADDR16 3
#define R_PPC_ADDR16_LO 4
#define R_PPC_ADDR16_HI 5
#define R_PPC_ADDR16_HA 6
#define R_PPC_ADDR14 7
#define R_PPC_ADDR14_BRTAKEN 8
#define R_PPC_ADDR14_BRNTAKEN 9
#define R_PPC_REL24 10
#define R_PPC_REL14 11
#define R_PPC_REL14_BRTAKEN 12
#define R_PPC_REL14_BRNTAKEN 13
#define R_RVL_NONE 201
#define R_RVL_SECT 202
#define R_RVL_STOP 203
#define R_RVL_MRKREF 204

namespace ninutils {

class RelHeader {
public:
    int32_t id;
    int32_t next;
    int32_t prev;
    int32_t numSections;
    int32_t sectionInfoOffset;
    int32_t nameOffset;
    int32_t nameSize;
    int32_t version;
    int32_t bssSize;
    int32_t relOffset;
    int32_t impOffset;
    int32_t impSize;
    uint8_t prologSection;
    uint8_t epilogSection;
    uint32_t unresolvedSection;
    uint32_t bssSection;
    uint32_t prolog;
    uint32_t epilog;
    uint32_t unresolved;
    uint32_t align;
    uint32_t bssAlign;
    int32_t fixSize;

    RelHeader(uint8_t* rel);
    friend std::ostream& operator<<(std::ostream& os, const RelHeader& rh);
};

class RelSection {
public:
    uint32_t offset;
    uint8_t unknown;
    uint8_t exec;
    uint32_t length;

    RelSection(uint8_t* sec);
    RelSection(const RelSection& other) : offset(other.offset), unknown(other.unknown),
        exec(other.exec), length(other.length) {
    }
    RelSection(RelSection&& other) noexcept : offset(other.offset), unknown(other.unknown),
        exec(other.exec), length(other.length) {
    }
    friend std::ostream& operator<<(std::ostream& os, const RelSection& rs);
};

class RelImpRaw {
public:
    int32_t module_id;
    /**
     * @brief Offset of relocation list from start of REL
     */
    uint32_t offset;

    RelImpRaw(uint8_t* rel);
    friend std::ostream& operator<<(std::ostream& os, const RelImpRaw& ri);
};

class RelImp {
public:
    int32_t module_id;
    /**
     * @brief Index in Rel#rels of start of relocations of module with RelImp#module_id
     */
    uint32_t index;
    /**
     * @brief Number of relocations of module with RelImp#module_id
     */
    uint32_t count;

    RelImp(int32_t module_id, uint32_t index, uint32_t count) : 
        module_id(module_id), index(index), count(count) {}
    friend std::ostream& operator<<(std::ostream& os, const RelImpRaw& ri);
};

/**
 * @brief Raw relocation data entry as read from the file
 */
class RelRelocRaw {
public:
    uint16_t offset;
    uint8_t type;
    uint8_t section;
    uint32_t addend;

    RelRelocRaw(uint8_t* rel);
    friend std::ostream& operator<<(std::ostream& os, const RelRelocRaw& rr);
};

/**
 * @brief Relocation data entry
 */
class RelReloc {
public:
    /**
     * @brief Section index of the relocation
     */
    uint8_t section_idx;
    /**
     * @brief Offset of relocation from RelReloc#section
     */
    uint32_t offset;
    /**
     * @brief Type of relocation
     */
    uint8_t type;

    /**
     * @brief Module of address to relocate against
     */
    int32_t src_module_id;
    /**
     * @brief Section of address to relocate against
     */
    uint8_t src_section_idx;
    /**
     * @brief Offset of address to relocate against from RelReloc#src_section
     */
    uint32_t src_offset;

    RelReloc(uint8_t section_idx, uint32_t offset, uint8_t type, int32_t src_module_id, uint8_t src_section_idx,
        uint32_t src_offset) : section_idx(section_idx), offset(offset), type(type),
        src_module_id(src_module_id), src_section_idx(src_section_idx), src_offset(src_offset) {}
    std::ostream& print(std::ostream& os) const;
};

class Rel {
public:
    // Raw data of REL file
    RelHeader hdr;
    std::vector<RelSection> secs_raw;
    std::vector<RelImpRaw> imps_raw;
    std::vector<RelRelocRaw> rels_raw;

    // More manageable representation of REL data
    std::vector<RelSection> secs;
    std::vector<RelImp> imps;
    std::vector<RelReloc> rels;
    
    // file data
    uint8_t* file = nullptr;
    size_t fileSize;

    /**
     * Load information. 0x0 if unknown
    */
    uint32_t load_addr;
    uint32_t bss_load_addr;
    Rel(uint8_t* rel, size_t size, std::optional<ExtraInfo> extra_info = std::nullopt);
    ~Rel();
    std::ostream& printRaw(std::ostream& os, bool print_relocs=false, bool p_hdr=true,
        bool p_secs=false, bool p_imps=false) const;
    std::ostream& print(std::ostream& os, bool print_relocs=false, bool p_hdr=true,
        bool p_secs=false, bool p_imps=false) const;
};
} // ns ninutils