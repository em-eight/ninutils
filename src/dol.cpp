
#include <algorithm>
#include <cassert>

#include "ninutils/dol.hpp"
#include "ninutils/utils.hpp"
#include "ninutils/common.hpp"

#define DOLHDR_OFFSETS_OFF 0x0
#define DOLHDR_OFFSET_SIZE 0x4
#define DOLHDR_ADDRESSES_OFF 0x48
#define DOLHDR_ADDRESS_SIZE 0x4
#define DOLHDR_SIZES_OFF 0x90
#define DOLHDR_SIZE_SIZE 0x4
#define DOLHDR_BSSADDR_OFF 0xd8
#define DOLHDR_BSSLEN_OFF 0xdc
#define DOLHDR_ENTRYPOINT_OFF 0xe0

namespace ninutils {

DolHeaderRaw::DolHeaderRaw(uint8_t* dol) {
    for (int i = 0; i < DOL_MAX_SECTION_COUNT; i++) {
        offsets[i] = readbe32(dol + DOLHDR_OFFSETS_OFF + i*DOLHDR_OFFSET_SIZE);
        addresses[i] = readbe32(dol + DOLHDR_ADDRESSES_OFF + i*DOLHDR_ADDRESS_SIZE);
        lengths[i] = readbe32(dol + DOLHDR_SIZES_OFF + i*DOLHDR_SIZE_SIZE);
    }

    bss_address = readbe32(dol + DOLHDR_BSSADDR_OFF);
    bss_length = readbe32(dol + DOLHDR_BSSLEN_OFF);
    entry_point = readbe32(dol + DOLHDR_ENTRYPOINT_OFF);
}

DolSection::DolSection(uint32_t offset, uint32_t address, uint32_t length, bool text, std::string name)
        : offset(offset), address(address), length(length), isText(text), name(name) {
}

void Dol::setSectionName(uint8_t sec, std::optional<ExtraInfo> extra_info) {
    if (extra_info.has_value() && extra_info->modules.find(0) != extra_info->modules.end()) {
        secs[sec].name = extra_info->modules[0].sections[sec].name;
        return;
    }

    // generate sensible default name
    uint8_t similarSectionTypeCount = 0;
    uint8_t sectionTypeIdx = 0;
    for (uint8_t i = 0; i < secs.size(); i++) {
        if (i == sec) continue;

        if (secs[i].isText == secs[sec].isText && secs[i].isBss() == secs[sec].isBss()) {
            similarSectionTypeCount++;
            if (i < sec) sectionTypeIdx++;
        }
    }
    std::string sectionBaseName;
    if (secs[sec].isText) {
        sectionBaseName = DEF_TEXT_SECTION_NAME;
    } else if (secs[sec].isBss()) {
        sectionBaseName = DEF_BSS_SECTION_NAME;
    } else {
        sectionBaseName = DEF_DATA_SECTION_NAME;
    }
    if (similarSectionTypeCount == 0) {
        secs[sec].name = sectionBaseName;
    } else {
        secs[sec].name = sectionBaseName + std::to_string(similarSectionTypeCount+1);
    }
}

Dol::Dol(uint8_t* dol, size_t size, std::optional<ExtraInfo> extra_info) : hdr(dol), fileSize(size) {
    secs.reserve(DOL_MAX_SECTION_COUNT);
    std::vector<uint32_t> dolSectionIdxsInterectingBss;
    for (int i = 0; i < DOL_MAX_SECTION_COUNT; i++) {
        if (hdr.offsets[i] == 0 || hdr.addresses[i] == 0 || hdr.lengths[i] == 0) {
            continue;
        }
        if (hdr.addresses[i] > hdr.bss_address && hdr.addresses[i] + hdr.lengths[i] < hdr.bss_address + hdr.bss_length) {
            dolSectionIdxsInterectingBss.push_back(i);
        } else {
            secs.emplace_back(hdr.offsets[i], hdr.addresses[i], hdr.lengths[i], i < 7);
            setSectionName(secs.size()-1, extra_info);
        }
    }
    // Handle SDA sections and bss
    if (dolSectionIdxsInterectingBss.size() == 0) {
        secs.emplace_back(0, hdr.bss_address, hdr.bss_length, false, ".bss");
    } else {
        std::sort(dolSectionIdxsInterectingBss.begin(), dolSectionIdxsInterectingBss.end(), [this](int a, int b) {
            return hdr.addresses[a] < hdr.addresses[b];
        });
        secs.emplace_back(0, hdr.bss_address, hdr.addresses[dolSectionIdxsInterectingBss[0]] - hdr.bss_address, false, ".bss");
        int count_sda = 1;
        for (int idx : dolSectionIdxsInterectingBss) {
            std::string suffix = count_sda != 1 ? std::to_string(count_sda) : std::string("");
            secs.emplace_back(hdr.offsets[idx], hdr.addresses[idx], hdr.lengths[idx], false, ".sdata" + suffix);

            uint32_t curr_sbss_start = hdr.addresses[idx] + hdr.lengths[idx];
            uint32_t curr_sbss_length = count_sda < dolSectionIdxsInterectingBss.size() ? hdr.addresses[idx+1]-curr_sbss_start : hdr.bss_length;
            assert(curr_sbss_length > 0);
            secs.emplace_back(0, curr_sbss_start, curr_sbss_length, false, ".sbss" + suffix);

            count_sda++;
        }
    }
    std::sort(secs.begin(), secs.end(), [](DolSection a, DolSection b) {
        return a.address < b.address;
    });
    secs.shrink_to_fit();

    file = new uint8_t[size];
    memcpy(file, dol, size);
}

Dol::~Dol() {
    if (file != nullptr)
        free(file);
}

std::optional<uint32_t> Dol::getSectionIdxByName(const std::string& name) {
    for (uint32_t i = 0; i < secs.size(); i++) {
        if (name == secs[i].name) {
            return std::optional<uint32_t>(i);
        }
    }
    return std::nullopt;
}

std::optional<uint32_t> Dol::getSectionIdxContainingAddress(uint32_t vma) {
    for (uint32_t i = 0; i < secs.size(); i++) {
        if (secs[i].address < vma && vma < secs[i].address + secs[i].length) {
            return std::optional<uint32_t>(i);
        }
    }
    return std::nullopt;
}

std::ostream& DolHeaderRaw::print(std::ostream& os) const {
    os << "Sections:" << "\n";
    os << WIDTH("Offset", 12) << WIDTH("Address", 12) << WIDTH("Size", 10) << "\n";
    for (int i = 0; i < DOL_MAX_SECTION_COUNT; i++) {
        os << HEX_FMTW(offsets[i], 12) << HEX_FMTW(addresses[i], 12) << HEX_FMTW(lengths[i], 12) << "\n";
    }
    os << WIDTH(".bss address", 14) << HEX_FMT(bss_address) << "\n";
    os << WIDTH(".bss length", 14) << HEX_FMT(bss_length) << "\n";
    os << WIDTH("entry point", 14) << HEX_FMT(entry_point) << "\n";
    return os;
}

std::ostream& DolSection::print(std::ostream& os) const {
    os << WIDTH(name, 12) << HEX_FMTW(offset, 12) << HEX_FMTW(address, 12) << HEX_FMTW(length, 12) << "\n";
    return os;
}

std::ostream& Dol::print(std::ostream& os) const {
    os << "Sections:" << "\n";
    os << WIDTH("Name", 12) << WIDTH("Offset", 12) << WIDTH("Address", 12) << WIDTH("Size", 10) << "\n";
    for (int i = 0; i < secs.size(); i++) {
        secs[i].print(os);
    }
    os << WIDTH(".bss address:", 15) << HEX_FMT(hdr.bss_address) << "\n";
    os << WIDTH(".bss length:", 15) << HEX_FMT(hdr.bss_length) << "\n";
    os << WIDTH("entry point:", 15) << HEX_FMT(hdr.entry_point) << "\n";
    return os;
}
} // ns ninutils
