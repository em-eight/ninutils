
#include "ninutils/symbols.hpp"
#include "ninutils/utils.hpp"

#include <algorithm>
#include <unordered_set>
#include <set>
    
void Symbol::genName() {
    std::stringstream stream;
    if (module_id == 0) {
        stream << "DOL_" << HEX_FMT(vma);
    } else {
        stream << "REL" << module_id << "." << DEC_FMT(NUM(section_idx)) << "_" << HEX_FMT(offset);
    }
    name = stream.str();
}

std::ostream& Symbol::print(std::ostream& os) const {
    // TODO: Support providing load address to calculate VMAs of REL symbols
    if (module_id == 0)
        os << DEC_FMTW(module_id, 12) << DEC_FMTW(NUM(section_idx), 9) << HEX_FMTW(offset, 12)
            << HEX_FMTW(vma, 12) << WIDTH(name, 12) << "\n";
    else
        os << DEC_FMTW(module_id, 12) << DEC_FMTW(NUM(section_idx), 9) << HEX_FMTW(offset, 12)
            << HEX_FMTW(vma, 12) << WIDTH("-", 12) << "\n";

    return os;
}

bool Symbol::operator==(const Symbol& other) const {
    if (this->module_id != other.module_id) return false;

    if (this->module_id == 0) { // DOL symbol
        return this->vma == other.vma;
    } else { // REL symbol
        return this->section_idx == other.section_idx && this->offset == other.offset;
    }
}

std::ostream& SymbolTable::print(std::ostream& os) const {
    os << "Symbols\n";
    os << WIDTH("Module ID", 12) << WIDTH("Section", 9) << WIDTH("Sec Offset", 12)
        << WIDTH("VMA", 12) << WIDTH("Name", 12) << "\n";
    for (const Symbol& sym : symbols)
        sym.print(os);

    return os;
}

SymbolTable::SymbolTable(const Rel& rel) {
    std::vector<Rel> rels;
    rels.emplace_back(rel);
    inferSyms(rels, getAllModules(rels));
}
    
std::vector<int32_t> SymbolTable::getAllModules(const std::vector<Rel>& rels) {
    std::vector<int32_t> module_ids;
    std::set<int32_t> module_ids_set;
    for (const Rel& rel : rels)
        for (const RelImp& imp : rel.imps)
            module_ids_set.insert(imp.module_id);

    module_ids.insert(module_ids.begin(), module_ids_set.begin(), module_ids_set.end());
    return module_ids;
}

SymbolTable::SymbolTable(const std::vector<Rel>& rels) {
    inferSyms(rels, getAllModules(rels));
}

void SymbolTable::inferSyms(const std::vector<Rel>& rels, const std::vector<int32_t>& module_ids) {
    std::unordered_set<Symbol, Symbol::HashFunction> added_symbols; // for fast lookup
    for (const Rel& rel : rels) {
        for (const RelImp& imp : rel.imps) {
            if (std::find(module_ids.begin(), module_ids.end(), imp.module_id) == module_ids.end())
                continue; // skip module

            for (int i = imp.index; i < imp.index + imp.count; i++) {
                const RelReloc& reloc = rel.rels[i];
                Symbol sym;
                if (imp.module_id == 0) { // DOL symbol
                    sym = Symbol(imp.module_id, reloc.src_offset);
                } else { // REL symbol
                    sym = Symbol(imp.module_id, reloc.section_idx, reloc.src_offset);
                }

                const auto& [it, inserted] = added_symbols.emplace(sym);
                if (inserted) {
                    symbols.emplace_back(sym);
                    symbols[symbols.size() - 1].genName();
                }
            }
        }
    }
}