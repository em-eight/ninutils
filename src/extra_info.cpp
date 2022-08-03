
#include "ninutils/extra_info.hpp"
#include "ninutils/utils.hpp"

#define MKW_PAL_PRESET_NAME "mkw-pal"
#define MKW_PAL_REL_LOAD_ADDR 0x805102e0
#define MKW_PAL_REL_BSS_LOAD_ADDR 0x809bd6e0

const char* preset_names[] = {MKW_PAL_PRESET_NAME};

ExtraInfo::ExtraInfo(std::string preset) {
    if (preset == MKW_PAL_PRESET_NAME) {
        description = "Mario Kart Wii PAL";
        ModuleExtraInfo relInfo(MKW_PAL_REL_LOAD_ADDR, MKW_PAL_REL_BSS_LOAD_ADDR);
        modules.insert({1, relInfo});
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