
#include "ninutils/dol.hpp"
#include "ninutils/utils.hpp"
#include "ninutils/symbols.hpp"

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <string>
#include <unistd.h>

class ReadDolArgs {
public:
    bool raw = false, hdr = false;
    std::string dolPath;
    std::string preset = "";

    int readArgs(int argc, char** argv) {
        int c;
        while ((c = getopt (argc, argv, "wp:")) != -1) {
            switch (c) {
            case 'w':
                raw = true;
                break;
            case 'p':
                preset = std::string(optarg);
                break;
            default:
                return -2;
            }
        }

        if (optind < argc) {
            dolPath = std::string(argv[optind]);
        } else {
            return -1;
        }
        return 0;
    }

    void printUsage(std::ostream& os) {
        os << "Usage: readdol <option(s)> dol_file\n"
            " Display information about the contents of DOL files\n"
            " Options are:\n"
            << WIDTH("\t-w", 5) << "Print in raw format, aka as is from the file\n"
            << WIDTH("\t-p <preset>", 14) << "Specify a preset to get game-specific info (e.g. VMAs of REL symbols)\n";

        ninutils::ExtraInfo::printPresets(os);
    }
};

int main(int argc, char** argv) {
    ReadDolArgs args;
    if (args.readArgs(argc, argv) < 0) {
        args.printUsage(std::cout);
        exit(-1);
    }

    std::ifstream dolstrm(args.dolPath, std::ios::binary | std::ios::in | std::ios::ate);
    if (!dolstrm.is_open()) {
        std::cout << "File " << argv[1] << " not found." << std::endl;
        exit(-2);
    } else {
        std::streamsize size = dolstrm.tellg();
        dolstrm.seekg(0, std::ios::beg);
        std::vector<char> buffer(size);
        dolstrm.read(buffer.data(), size);

        ninutils::ExtraInfo extra_info(args.preset);
        ninutils::Dol dol((uint8_t*) buffer.data(), size,
            extra_info.description.empty() ? std::nullopt : std::optional<ninutils::ExtraInfo>(extra_info));
        if (args.raw) {
            dol.hdr.print(std::cout);
        } else {
            dol.print(std::cout);
        }
    }
}
