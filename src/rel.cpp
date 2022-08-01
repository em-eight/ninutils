
#include "ninutils/utils.hpp"
#include "ninutils/rel.hpp"

#include <cstring>




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
    os << "ID:\t"                << DEC_FMT(relhdr.id)                << "\n"
       << "next:\t"              << HEX_FMT(relhdr.next)              << "\n"
       << "prev:\t"              << HEX_FMT(relhdr.prev)              << "\n"
       << "numSections:\t"       << DEC_FMT(relhdr.numSections)       << "\n"
       << "sectionInfoOffset:\t" << HEX_FMT(relhdr.sectionInfoOffset) << "\n"
       << "nameOffset:\t"        << HEX_FMT(relhdr.nameOffset)        << "\n"
       << "nameSize:\t"          << DEC_FMT(relhdr.nameSize)          << "\n"
       << "version:\t"           << DEC_FMT(relhdr.version)           << "\n"
       << "bssSize:\t"           << HEX_FMT(relhdr.bssSize)           << "\n"
       << "relOffset:\t"         << HEX_FMT(relhdr.relOffset)         << "\n"
       << "impOffset:\t"         << HEX_FMT(relhdr.impOffset)         << "\n"
       << "impSize:\t"           << HEX_FMT(relhdr.impSize)           << "\n"
       << "prologSection:\t"     << DEC_FMT(NUM(relhdr.prologSection))     << "\n"
       << "epilogSection:\t"     << DEC_FMT(NUM(relhdr.epilogSection))     << "\n"
       << "unresolvedSection:\t" << DEC_FMT(NUM(relhdr.unresolvedSection)) << "\n"
       << "bssSection:\t"        << DEC_FMT(NUM(relhdr.bssSection))        << "\n"
       << "prolog:\t"            << HEX_FMT(relhdr.prolog)            << "\n"
       << "epilog:\t"            << HEX_FMT(relhdr.epilog)            << "\n"
       << "unresolved:\t"        << HEX_FMT(relhdr.unresolved)        << "\n"
       << "align:\t"             << HEX_FMT(relhdr.align)             << "\n"
       << "bssAlign:\t"          << HEX_FMT(relhdr.bssAlign)          << "\n"
       << "fixSize:\t"           << HEX_FMT(relhdr.fixSize)           << "\n";
    return os;
}

RelSection::RelSection(uint8_t* sec) {
    offset = 0xfffffffc & readbe32(sec);
    unknown = (readbe32(sec) >> 1) & 0x1;
    exec = (readbe32(sec) >> 0) & 0x1;
    size = readbe32(sec + 0x4);
}

std::ostream& operator<<(std::ostream& os, const RelSection& relsec)
{
    os << HEX_FMT(relsec.offset)  << "\t" << BOOL_FMT(relsec.unknown) << "\t" << BOOL_FMT(NUM(relsec.exec)) 
        << "\t" << HEX_FMT(relsec.size) << "\n";
    return os;
}

RelImpRaw::RelImpRaw(uint8_t* imp) {
    module_id = readbe32(imp + RELIMP_MODULEID_OFF);
    offset = readbe32(imp + RELIMP_OFFSET_OFF);
}

std::ostream& operator<<(std::ostream& os, const RelImpRaw& relimp)
{
    os << DEC_FMT(relimp.module_id)  << "\t\t" << HEX_FMT(relimp.offset) << "\n";
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
    case R_PPC_ADDR14_8:
        return "R_RVL_ADDR14";
    case R_PPC_ADDR14_9:
        return "R_RVL_ADDR14";
    case R_PPC_REL24:
        return "R_RVL_REL24";
    case R_PPC_REL14:
        return "R_RVL_REL14";
    case R_PPC_REL14_12:
        return "R_RVL_REL14";
    case R_PPC_REL14_13:
        return "R_RVL_REL14";
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
    // TODO: relocation type to string
    os << HEX_FMT(relrel.offset)  << "\t" << relocTypeToString(relrel.type) << "\t"
        << DEC_FMT(NUM(relrel.section)) << "\t" << HEX_FMT(relrel.addend) << "\n";
    return os;
}

Rel::Rel(uint8_t* rel) : hdr(rel) {
    secs.reserve(hdr.numSections);
    for (int i = 0; i < hdr.numSections; i++) {
        secs.emplace_back(RelSection(rel + hdr.sectionInfoOffset + i*REL_SECTION_INFO_SIZE));
    }

    uint32_t numImps = hdr.impSize / REL_IMP_SIZE;
    imps.reserve(numImps);
    for (int i = 0; i < numImps; i++) {
        imps.emplace_back(RelImpRaw(rel + hdr.impOffset + i*REL_IMP_SIZE));
    }

    for (int i = 0; i < imps.size(); i++) {
        uint8_t type = R_PPC_NONE;
        uint32_t it = 0;
        while (type != R_RVL_STOP) {
            RelRelocRaw reloc(rel + imps[i].offset + it);
            rels.emplace_back(reloc);
            type = reloc.type;
            it += REL_REL_SIZE;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Rel& rel)
{
    os << rel.hdr;

    os << "\nSections:\n";
    os << "Offset\tUnknown Executable Size\n";
    for (const RelSection& sec : rel.secs)
        os << sec;

    os << "\nImps:\n";
    os << "Module ID\tOffset\n";
    for (const RelImpRaw& imp : rel.imps)
        os << imp;
    
    os << "\nRelocations:\n";
    os << "Offset\ttype section added\n";
    for (const RelRelocRaw& rel : rel.rels)
        os << rel;
    return os;
}