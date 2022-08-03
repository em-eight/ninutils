
#pragma once

#include <cstdint>
#include <map>

#define MKW_PAL_REL_LOAD_ADDR 0x805102e0

class ModuleExtraInfo {
public:
    /**
     * @brief The virtual memory address the module is loaded at.
     * For REL, this is the VMA the start of the header is mapped at. 0x0 is unknown/invalid 
     */
    uint32_t load_address;

    ModuleExtraInfo() : load_address(0x0) {} // default constructor
    ModuleExtraInfo(uint32_t load_address) : load_address(load_address) {}
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
};