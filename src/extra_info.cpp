
#include "ninutils/extra_info.hpp"
#include "ninutils/utils.hpp"

#define MKW_PAL_PRESET_NAME "mkw-pal"
#define MKW_PAL_REL_LOAD_ADDR 0x805102e0
#define MKW_PAL_REL_BSS_LOAD_ADDR 0x809bd6e0

namespace ninutils {

const char* preset_names[] = {MKW_PAL_PRESET_NAME};

ExtraInfo::ExtraInfo(std::string preset) {
    if (preset == MKW_PAL_PRESET_NAME) {
        description = "Mario Kart Wii PAL";
        ModuleExtraInfo relInfo(MKW_PAL_REL_LOAD_ADDR, MKW_PAL_REL_BSS_LOAD_ADDR);
        relInfo.sections.emplace(0, ".text");
        relInfo.sections.emplace(1, ".ctors");
        relInfo.sections.emplace(2, ".dtors");
        relInfo.sections.emplace(3, ".rodata");
        relInfo.sections.emplace(4, ".data");
        relInfo.sections.emplace(5, ".bss");
        modules.insert({1, relInfo});

        ModuleExtraInfo dolInfo;
        dolInfo.sections.emplace(0, ".init");
        dolInfo.sections.emplace(1, ".text");
        dolInfo.sections.emplace(2, "extab");
        dolInfo.sections.emplace(3, "extabindex");
        dolInfo.sections.emplace(4, ".ctors");
        dolInfo.sections.emplace(5, ".dtors");
        dolInfo.sections.emplace(6, ".rodata");
        dolInfo.sections.emplace(7, ".data");
        dolInfo.sections.emplace(8, ".bss");
        dolInfo.sections.emplace(9, ".sdata");
        dolInfo.sections.emplace(10, ".sbss");
        dolInfo.sections.emplace(11, ".sdata2");
        dolInfo.sections.emplace(12, ".sbss2");
        modules.insert({0, dolInfo});
    }
}

std::ostream& ExtraInfo::printPresets(std::ostream& os) {
    os << "Available presets:\n";
    for (const char* preset_name : preset_names) {
        ExtraInfo extra_info(preset_name);
        os << WIDTH(preset_name, 12) << extra_info.description << "\n";
    }
    return os;
}
}