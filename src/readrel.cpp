
#include "ninutils/rel.hpp"
#include "ninutils/utils.hpp"

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <string>
#include <unistd.h>

class ReadRelArgs {
public:
    bool raw = false, hdr = false, secs = false, rels = false, imps = false;
    std::string relPath;

    int readArgs(int argc, char** argv) {
        int c;
        while ((c = getopt (argc, argv, "whSri")) != -1) {
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
            default:
                return -2;
            }
        }

        if (optind < argc) {
            relPath = std::string(argv[optind]);
        } else {
            return -1;
        }
        if (!rels && !secs && !hdr && !imps)
            return -3;
        return 0;
    }

    void printUsage(std::ostream& os) {
        os << "Usage: readrel <option(s)> rel_file\n"
            " Display information about the contents of REL files\n"
            " Options are:\n"
            << WIDTH("\t-h", 5) << "Print REL header\n"
            << WIDTH("\t-S", 5) << "Print REL section table\n"
            << WIDTH("\t-i", 5) << "Print REL imp table\n"
            << WIDTH("\t-r", 5) << "Print REL relocations\n"
            << WIDTH("\t-w", 5) << "Print in raw format, aka as is from the file\n";
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

        Rel rel((uint8_t*) buffer.data());
        if (args.raw) {
            rel.printRaw(std::cout, args.rels, args.hdr, args.secs, args.imps);
        } else {
            rel.print(std::cout, args.rels, args.hdr, args.secs, args.imps);
        }
    }
}
