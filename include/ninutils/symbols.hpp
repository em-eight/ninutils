
#pragma once

#include "rel.hpp"

class Symbol {
public:
    int32_t module_id;
    /**
     * @brief REL symbols only. Section index in Rel#secs_raw.
     */
    uint8_t section_idx;
    /**
     * @brief REL symbols only. Offset of symbol from section.
     */
    uint32_t offset;
    /**
     * @brief virtual memory address of symbol.
     * Always valid for DOL symbols, only valid for REL symbols if its load address is known
     */
    uint32_t vma;
    std::string name;

    Symbol() = default;
    Symbol(int32_t module_id, uint8_t section_idx, uint32_t offset, uint32_t vma = 0x0) :
        module_id(module_id), section_idx(section_idx), offset(offset), vma(vma) {}
    Symbol(int32_t module_id, uint32_t vma) : module_id(module_id), vma(vma) {}
    void genName();
    bool operator==(const Symbol& other) const;
    std::ostream& print(std::ostream& os) const;
    // required for unordered_set
    struct HashFunction {
        size_t operator()(const Symbol& sym) const {
            size_t secHash = std::hash<uint8_t>()(sym.section_idx);
            size_t offHash = std::hash<uint32_t>()(sym.offset) << 1;
            size_t vmaHash = std::hash<uint32_t>()(sym.vma) << 2;
            return secHash ^ offHash ^ vmaHash;
        }
    };
};

class SymbolTable {
public:
    std::vector<Symbol> symbols;

    /**
     * @brief Infer a symbol table from REL files
     * 
     * @param rels REL files to infer symbols from
     */
    SymbolTable(const std::vector<Rel>& rels);
    // convenience overload for only one REL
    SymbolTable(const Rel& rel);
    /**
     * @brief Infer a symbol table from REL files, only look at symbols from provided module IDs
     * 
     * @param rels REL files to infer symbols from
     * @param module_ids Module IDs of interest
     */
    void inferSyms(const std::vector<Rel>& rels, const std::vector<int32_t>& module_ids);
    std::ostream& print(std::ostream& os) const;

private:
    static std::vector<int32_t> getAllModules(const std::vector<Rel>& rels);
};