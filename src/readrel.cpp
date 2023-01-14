
#include "ninutils/rel.hpp"
#include "ninutils/utils.hpp"
#include "ninutils/symbols.hpp"
#include "ninutils/extra_info.hpp"

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <string>
#include <unistd.h>

class ReadRelArgs {
public:
    bool raw = false, hdr = false, secs = false, rels = false, imps = false, syms = false;
    std::string relPath;
    std::string preset = "";

    int readArgs(int argc, char** argv) {
        int c;
        while ((c = getopt (argc, argv, "whSrisp:")) != -1) {
            switch (c) {
            case 'w':
                raw = true;
                break;
            case 'h':
                hdr = true;
                break;
            case 'S':
                secs = true;
                break;
            case 'r':
                rels = true;
                break;
            case 'i':
                imps = true;
                break;
            case 's':
                syms = true;
                break;
            case 'p':
                preset = std::string(optarg);
                break;
            default:
                return -2;
            }
        }

        if (optind < argc) {
            relPath = std::string(argv[optind]);
        } else {
            return -1;
        }
        if (!rels && !secs && !hdr && !imps && !syms)
            return -3;
        return 0;
    }

    void printUsage(std::ostream& os) {
        os << "Usage: readrel <option(s)> rel_file\n"
            " Display information about the contents of REL files\n"
            " Options are:\n"
            << WIDTH("\t-h", 14) << "Print REL header\n"
            << WIDTH("\t-S", 14) << "Print REL section table\n"
            << WIDTH("\t-i", 14) << "Print REL imp table\n"
            << WIDTH("\t-r", 14) << "Print REL relocations\n"
            << WIDTH("\t-s", 14) << "Print REL inferred symbols\n"
            << WIDTH("\t-w", 14) << "Print in raw format, aka as is from the file\n"
            << WIDTH("\t-p <preset>", 14) << "Specify a preset to get game-specific info (e.g. VMAs of REL symbols)\n";

        ninutils::ExtraInfo::printPresets(os);
    }
};

int main(int argc, char** argv) {
    ReadRelArgs args;
    if (args.readArgs(argc, argv) < 0) {
        args.printUsage(std::cout);
        exit(-1);
    }

    std::ifstream relstrm(args.relPath, std::ios::binary | std::ios::in | std::ios::ate);
    if (!relstrm.is_open()) {
        std::cout << "File " << argv[1] << " not found." << std::endl;
        exit(-2);
    } else {
        std::streamsize size = relstrm.tellg();
        relstrm.seekg(0, std::ios::beg);
        std::vector<char> buffer(size);
        relstrm.read(buffer.data(), size);

        ninutils::ExtraInfo extra_info(args.preset);
        ninutils::Rel rel((uint8_t*) buffer.data(),
            extra_info.description.empty() ? std::nullopt : std::optional<ninutils::ExtraInfo>(extra_info));
        if (args.raw) {
            rel.printRaw(std::cout, args.rels, args.hdr, args.secs, args.imps);
        } else {
            rel.print(std::cout, args.rels, args.hdr, args.secs, args.imps);
        }

        if (args.syms) {
            ninutils::SymbolTable symtab(rel);
            symtab.print(std::cout);
        }
    }
}
