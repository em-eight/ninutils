
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <ostream>

namespace ninutils {

class SectionExtraInfo {
public:
    std::string name;

    SectionExtraInfo() : name("") {} // default constructor
    SectionExtraInfo(std::string name) : name(name) {}
};

class ModuleExtraInfo {
public:
    std::map<int32_t, SectionExtraInfo> sections;
    /**
     * @brief The virtual memory address the module is loaded at.
     * For REL, this is the VMA the start of the header is mapped at. 0x0 is unknown/invalid 
     */
    uint32_t load_address;
    /**
     * @brief REL only. The virtual memory address of the .bss section
     */
    uint32_t bss_load_address;

    ModuleExtraInfo() : load_address(0x0), bss_load_address(0x0) {} // default constructor
    ModuleExtraInfo(uint32_t load_address, uint32_t bss_load_address) 
        : load_address(load_address), bss_load_address(bss_load_address) {}
};

class ExtraInfo {
public:
    std::map<int32_t, ModuleExtraInfo> modules;
    // Brief human-readable description for preset. Empty ExtraInfo's description is the empty string
    std::string description;

    ExtraInfo(std::map<int32_t, ModuleExtraInfo> modules) : modules(modules) {}
    /**
     * @brief Construct a new Extra_Info object from preset if found, else return empty ExtraInfo
     * @param preset Name of the preset
     */
    ExtraInfo(std::string preset);
    static std::ostream& printPresets(std::ostream& os); 
};

extern const char* preset_names[];

} // ns ninutils