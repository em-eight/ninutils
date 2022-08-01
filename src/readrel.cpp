
#include "ninutils/rel.hpp"

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    std::ifstream relstrm(std::string(argv[1]), std::ios::binary | std::ios::in | std::ios::ate);
    if (!relstrm.is_open()) {
        std::cout << "File " << argv[1] << " not found." << std::endl;
        exit(-1);
    } else {
        std::streamsize size = relstrm.tellg();
        relstrm.seekg(0, std::ios::beg);
        std::vector<char> buffer(size);
        relstrm.read(buffer.data(), size);

        Rel rel((uint8_t*) buffer.data());
        rel.print(std::cout);
    }
}
