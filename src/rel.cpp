
#include "ninutils/utils.hpp"
#include "ninutils/rel.hpp"

#include <cstring>

#define RELHDR_ID_OFF 0x00
#define RELHDR_ID_SIZE 0x04
#define RELHDR_NEXT_OFF 0x04
#define RELHDR_NEXT_SIZE 0x04
#define RELHDR_PREV_OFF 0x08
#define RELHDR_PREV_SIZE 0x04
#define RELHDR_NUMSECTIONS_OFF 0x0c
#define RELHDR_NUMSECTIONS_SIZE 0x04
#define RELHDR_SECTIONINFOOFFSET_OFF 0x10
#define RELHDR_SECTIONINFOOFFSET_SIZE 0x04
#define RELHDR_NAMEOFFSET_OFF 0x14
#define RELHDR_NAMEOFFSET_SIZE 0x04
#define RELHDR_NAMESIZE_OFF 0x18
#define RELHDR_NAMESIZE_SIZE 0x04
#define RELHDR_VERSION_OFF 0x1c
#define RELHDR_VERSION_SIZE 0x04
#define RELHDR_BSSSIZE_OFF 0x20
#define RELHDR_BSSSIZE_SIZE 0x04
#define RELHDR_RELOFFSET_OFF 0x24
#define RELHDR_RELOFFSET_SIZE 0x04
#define RELHDR_IMPOFFSET_OFF 0x28
#define RELHDR_IMPOFFSET_SIZE 0x04
#define RELHDR_IMPSIZE_OFF 0x2c
#define RELHDR_IMPSIZE_SIZE 0x04
#define RELHDR_PROLOGSECTION_OFF 0x30
#define RELHDR_PROLOGSECTION_SIZE 0x01
#define RELHDR_EPILOGSECTION_OFF 0x31
#define RELHDR_EPILOGSECTION_SIZE 0x01
#define RELHDR_UNRESOLVEDSECTION_OFF 0x32
#define RELHDR_UNRESOLVEDSECTION_SIZE 0x01
#define RELHDR_BSSSECTION_OFF 0x33
#define RELHDR_BSSSECTION_SIZE 0x01
#define RELHDR_PROLOG_OFF 0x34
#define RELHDR_PROLOG_SIZE 0x04
#define RELHDR_EPILOG_OFF 0x38
#define RELHDR_EPILOG_SIZE 0x04
#define RELHDR_UNRESOLVED_OFF 0x3c
#define RELHDR_UNRESOLVED_SIZE 0x04
#define RELHDR_ALIGN_OFF 0x40
#define RELHDR_ALIGN_SIZE 0x04
#define RELHDR_BSSALIGN_OFF 0x44
#define RELHDR_BSSALIGN_SIZE 0x04
#define RELHDR_FIXSIZE_OFF 0x48
#define RELHDR_FIXSIZE_SIZE 0x04

#define REL_SECTION_INFO_SIZE 0x8
#define REL_IMP_SIZE 0x8
#define REL_REL_SIZE 0x8

#define RELIMP_MODULEID_OFF 0x0
#define RELIMP_OFFSET_OFF 0x4

#define RELREL_OFFSET_OFF 0x0
#define RELREL_TYPE_OFF 0x2
#define RELREL_SECTION_OFF 0x3
#define RELREL_ADDEND_OFF 0x4


namespace ninutils{

RelHeader::RelHeader(uint8_t* rel) {
    id = readbe32(rel + RELHDR_ID_OFF);
    next = readbe32(rel + RELHDR_NEXT_OFF);
    prev = readbe32(rel + RELHDR_PREV_OFF);
    numSections = readbe32(rel + RELHDR_NUMSECTIONS_OFF);
    sectionInfoOffset = readbe32(rel + RELHDR_SECTIONINFOOFFSET_OFF);
    nameOffset = readbe32(rel + RELHDR_NAMEOFFSET_OFF);
    nameSize = readbe32(rel + RELHDR_NAMESIZE_OFF);
    version = readbe32(rel + RELHDR_VERSION_OFF);
    bssSize = readbe32(rel + RELHDR_BSSSIZE_OFF);
    relOffset = readbe32(rel + RELHDR_RELOFFSET_OFF);
    impOffset = readbe32(rel + RELHDR_IMPOFFSET_OFF);
    impSize = readbe32(rel + RELHDR_IMPSIZE_OFF);
    prologSection = readbe8(rel + RELHDR_PROLOGSECTION_OFF);
    epilogSection = readbe8(rel + RELHDR_EPILOGSECTION_OFF);
    unresolvedSection = readbe8(rel + RELHDR_UNRESOLVEDSECTION_OFF);
    bssSection = readbe8(rel + RELHDR_BSSSECTION_OFF);
    prolog = readbe32(rel + RELHDR_PROLOG_OFF);
    epilog = readbe32(rel + RELHDR_EPILOG_OFF);
    unresolved = readbe32(rel + RELHDR_UNRESOLVED_OFF);
    align = readbe32(rel + RELHDR_ALIGN_OFF);
    bssAlign = readbe32(rel + RELHDR_BSSALIGN_OFF);
    fixSize = readbe32(rel + RELHDR_FIXSIZE_OFF);
}

std::ostream& operator<<(std::ostream& os, const RelHeader& relhdr)
{
    os << WIDTH("ID:", 20)                << DEC_FMT(relhdr.id)                << "\n"
       << WIDTH("next:", 20)              << HEX_FMT(relhdr.next)              << "\n"
       << WIDTH("prev:", 20)              << HEX_FMT(relhdr.prev)              << "\n"
       << WIDTH("numSections:", 20)       << DEC_FMT(relhdr.numSections)       << "\n"
       << WIDTH("sectionInfoOffset:", 20) << HEX_FMT(relhdr.sectionInfoOffset) << "\n"
       << WIDTH("nameOffset:", 20)        << HEX_FMT(relhdr.nameOffset)        << "\n"
       << WIDTH("nameSize:", 20)          << DEC_FMT(relhdr.nameSize)          << "\n"
       << WIDTH("version:", 20)           << DEC_FMT(relhdr.version)           << "\n"
       << WIDTH("bssSize:", 20)           << HEX_FMT(relhdr.bssSize)           << "\n"
       << WIDTH("relOffset:", 20)         << HEX_FMT(relhdr.relOffset)         << "\n"
       << WIDTH("impOffset:", 20)         << HEX_FMT(relhdr.impOffset)         << "\n"
       << WIDTH("impSize:", 20)           << HEX_FMT(relhdr.impSize)           << "\n"
       << WIDTH("prologSection:", 20)     << DEC_FMT(NUM(relhdr.prologSection))     << "\n"
       << WIDTH("epilogSection:", 20)     << DEC_FMT(NUM(relhdr.epilogSection))     << "\n"
       << WIDTH("unresolvedSection:", 20) << DEC_FMT(NUM(relhdr.unresolvedSection)) << "\n"
       << WIDTH("bssSection:", 20)        << DEC_FMT(NUM(relhdr.bssSection))        << "\n"
       << WIDTH("prolog:", 20)            << HEX_FMT(relhdr.prolog)            << "\n"
       << WIDTH("epilog:", 20)            << HEX_FMT(relhdr.epilog)            << "\n"
       << WIDTH("unresolved:", 20)        << HEX_FMT(relhdr.unresolved)        << "\n"
       << WIDTH("align:", 20)             << HEX_FMT(relhdr.align)             << "\n"
       << WIDTH("bssAlign:", 20)          << HEX_FMT(relhdr.bssAlign)          << "\n"
       << WIDTH("fixSize:", 20)           << HEX_FMT(relhdr.fixSize)           << "\n";
    return os;
}

RelSection::RelSection(uint8_t* sec) {
    offset = 0xfffffffc & readbe32(sec);
    unknown = (readbe32(sec) >> 1) & 0x1;
    exec = (readbe32(sec) >> 0) & 0x1;
    length = readbe32(sec + 0x4);
}

std::ostream& operator<<(std::ostream& os, const RelSection& relsec)
{
    os << HEX_FMTW(relsec.offset, 12)  << BOOL_FMTW(relsec.unknown, 10)
        << BOOL_FMTW(NUM(relsec.exec), 12) << HEX_FMTW(relsec.length, 12) << "\n";
    return os;
}

RelImpRaw::RelImpRaw(uint8_t* imp) {
    module_id = readbe32(imp + RELIMP_MODULEID_OFF);
    offset = readbe32(imp + RELIMP_OFFSET_OFF);
}

std::ostream& operator<<(std::ostream& os, const RelImpRaw& relimp) {
    os << DEC_FMT(relimp.module_id)  << "\t\t" << HEX_FMT(relimp.offset) << "\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const RelImp& relimp) {
    os << DEC_FMT(relimp.module_id)  << "\t\t" << DEC_FMT(relimp.index) << "\t\t"
        << DEC_FMT(relimp.count) << "\n";
    return os;
}

RelRelocRaw::RelRelocRaw(uint8_t* rel) {
    offset = readbe16(rel + RELREL_OFFSET_OFF);
    type = readbe8(rel + RELREL_TYPE_OFF);
    section = readbe8(rel + RELREL_SECTION_OFF);
    addend = readbe32(rel + RELREL_ADDEND_OFF);
}

std::string relocTypeToString(uint8_t type) {
    switch (type)
    {
    case R_PPC_NONE:
        return "R_RVL_NONE";
    case R_PPC_ADDR32:
        return "R_RVL_ADDR32";
    case R_PPC_ADDR24:
        return "R_RVL_ADDR24";
    case R_PPC_ADDR16:
        return "R_RVL_ADDR16";
    case R_PPC_ADDR16_LO:
        return "R_RVL_ADDR16_LO";
    case R_PPC_ADDR16_HI:
        return "R_RVL_ADDR16_HI";
    case R_PPC_ADDR16_HA:
        return "R_RVL_ADDR16_HA";
    case R_PPC_ADDR14:
        return "R_RVL_ADDR14";
    case R_PPC_ADDR14_BRTAKEN:
        return "R_RVL_ADDR14_BRTAKEN";
    case R_PPC_ADDR14_BRNTAKEN:
        return "R_RVL_ADDR14_BRNTAKEN";
    case R_PPC_REL24:
        return "R_RVL_REL24";
    case R_PPC_REL14:
        return "R_RVL_REL14";
    case R_PPC_REL14_BRTAKEN:
        return "R_RVL_REL14_BRTAKEN";
    case R_PPC_REL14_BRNTAKEN:
        return "R_RVL_REL14_BRNTAKEN";
    case R_RVL_NONE:
        return "R_RVL_NONE";
    case R_RVL_SECT:
        return "R_RVL_SECT";
    case R_RVL_STOP:
        return "R_RVL_STOP";
    case R_RVL_MRKREF:
        return "R_RVL_MRKREF";
    default:
        return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, const RelRelocRaw& relrel) {
    os << HEX_FMTW(relrel.offset, 12)  << WIDTH(relocTypeToString(relrel.type), 17)
        << DEC_FMTW(NUM(relrel.section), 9) << "\t" << HEX_FMTW(relrel.addend, 12) << "\n";
    return os;
}

std::ostream& RelReloc::print(std::ostream& os) const {
    os << DEC_FMTW(NUM(section_idx), 9) << HEX_FMTW(offset, 12) << WIDTH(relocTypeToString(type), 17)
        << DEC_FMTW(NUM(src_module_id), 11) << "\t" << DEC_FMTW(NUM(src_section_idx), 9) << "\t" 
        << HEX_FMTW(src_offset, 12) << "\n";
    return os;
}

Rel::Rel(uint8_t* rel, size_t size, std::optional<ExtraInfo> extra_info) : hdr(rel), fileSize(size) {
    secs_raw.reserve(hdr.numSections);
    for (int i = 0; i < hdr.numSections; i++) {
        RelSection sec(rel + hdr.sectionInfoOffset + i*REL_SECTION_INFO_SIZE);
        secs_raw.emplace_back(sec);
        if (sec.length > 0) secs.emplace_back(sec);
    }

    uint32_t numImps = hdr.impSize / REL_IMP_SIZE;
    imps_raw.reserve(numImps);
    imps.reserve(numImps);
    for (int i = 0; i < numImps; i++) {
        imps_raw.emplace_back(rel + hdr.impOffset + i*REL_IMP_SIZE);
        // index and count set when parsing relocations
        imps.emplace_back(imps_raw[i].module_id, 0, 0);
    }

    uint32_t reloc_count = 0;
    for (int i = 0; i < imps_raw.size(); i++) {
        uint32_t it = 0;

        uint32_t module_reloc_count = 0;
        uint32_t section_idx = 0;
        uint32_t section_offset = 0;
        imps[i].index = reloc_count;
        while (true) {
            RelRelocRaw reloc(rel + imps_raw[i].offset + it);
            rels_raw.emplace_back(reloc);
            if (reloc.type == R_RVL_STOP) {
                break;
            } else if (reloc.type == R_RVL_SECT) {
                section_idx = reloc.section;
                section_offset = 0;
            } else if (reloc.type == R_PPC_NONE) { //skip
            } else if (reloc.type == R_RVL_NONE) {
                section_offset += reloc.offset;
            } else {
                section_offset += reloc.offset;
                rels.emplace_back(section_idx, section_offset, reloc.type,
                    imps_raw[i].module_id, reloc.section, reloc.addend);
                module_reloc_count++;
            }
            it += REL_REL_SIZE;
        }
        imps[i].count = module_reloc_count;
        reloc_count += module_reloc_count;
    }
    rels_raw.shrink_to_fit();
    rels.shrink_to_fit();

    // Get extra info from ExtraInfo if provided
    if (extra_info.has_value() && extra_info->modules.count(hdr.id) > 0) {
        load_addr = extra_info->modules[hdr.id].load_address;
        bss_load_addr = extra_info->modules[hdr.id].bss_load_address;
    } else {
        load_addr = 0x0;
        bss_load_addr = 0x0;
    }
    
    memcpy(file, rel, size);
}

Rel::~Rel() {
    if (file)
        free(file);
}

std::ostream& Rel::printRaw(std::ostream& os, bool print_relocs, bool p_hdr, bool p_secs, bool p_imps) const {
    if (p_hdr)
        os << hdr;

    if (p_secs) {
        os << "\nSections:\n";
        os << WIDTH("Offset", 12) << WIDTH("Unknown",10) << WIDTH("Executable",12) <<  WIDTH("Size",12) << "\n";
        for (const RelSection& sec : secs_raw)
            os << sec;
    }

    if (p_imps) {
        os << "\nImps:\n";
        os << "Module ID\tOffset\n";
        for (const RelImpRaw& imp : imps_raw)
            os << imp;
    }
    
    if (print_relocs) {
        os << "\nRelocations:\n";
        os << "Offset\ttype section added\n";
        for (const RelRelocRaw& rel : rels_raw)
            os << rel;
    }
    return os;
}

std::ostream& Rel::print(std::ostream& os, bool p_relocs, bool p_hdr, bool p_secs, bool p_imps) const {
    if (p_hdr)
        os << hdr;

    if (p_secs) {
        os << "\nSections:\n";
        os << WIDTH("Offset", 12) << WIDTH("Unknown",10) << WIDTH("Executable",12) <<  WIDTH("Size",12) << "\n";
        for (const RelSection& sec : secs) {
            os << sec;
        }
    }

    if (p_imps) {
        os << "\nImps:\n";
        os << "Module ID\tIndex\t\tCount\n";
        for (const RelImp& imp : imps)
            os << imp;
    }
    
    if (p_relocs) {
        os << "\nRelocations:\n";
        os << "dst sec\tdst off\ttype\tmodule ID\tsrc sec\tsrc off\n";
        for (const RelReloc& rel : rels)
            rel.print(os);
    }
    return os;
}
} // ns ninutils