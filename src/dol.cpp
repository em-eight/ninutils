
#include "ninutils/dol.hpp"
#include "ninutils/utils.hpp"

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

Dol::Dol(uint8_t* dol) : hdr(dol) {
    secs.reserve(DOL_MAX_SECTION_COUNT);
    for (int i = 0; i < DOL_MAX_SECTION_COUNT; i++) {
        if (hdr.offsets[i] != 0 && hdr.addresses[i] != 0 && hdr.lengths[i] != 0) {
            secs.emplace_back(DolSection(hdr.offsets[i], hdr.addresses[i], hdr.lengths[i]));
        }
    }
    secs.shrink_to_fit();
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
    os << HEX_FMTW(offset, 12) << HEX_FMTW(address, 12) << HEX_FMTW(length, 12) << "\n";
    return os;
}

std::ostream& Dol::print(std::ostream& os) const {
    os << "Sections:" << "\n";
    os << WIDTH("Offset", 12) << WIDTH("Address", 12) << WIDTH("Size", 10) << "\n";
    for (int i = 0; i < secs.size(); i++) {
        secs[i].print(os);
    }
    os << WIDTH(".bss address", 14) << HEX_FMT(hdr.bss_address) << "\n";
    os << WIDTH(".bss length", 14) << HEX_FMT(hdr.bss_length) << "\n";
    os << WIDTH("entry point", 14) << HEX_FMT(hdr.entry_point) << "\n";
    return os;
}