
#include "ninutils/presets.hpp"

ExtraInfo::ExtraInfo(std::string preset) {
    if (preset == "mkw-pal") {
        description = "Mario Kart Wii PAL";
        ModuleExtraInfo relInfo(MKW_PAL_REL_LOAD_ADDR);
        modules.insert({1, relInfo});
    }
}