#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>
#include <cstdlib>

#include <cstdint>
#include <cstddef>
#include <span>
#include <vector>
#include <filesystem>
#include <iostream>

#include "d2s/D2SFile.hpp"
#include "common/FlagSet.hpp"

int main(int const argc, char const* const* argv)
{
    if (argc<2) {
        std::cerr << "no file provided\n";
        return EXIT_FAILURE;
    }

    try {
        D2SFile d2sFile {argv[1]};

        D2SFile::ErrorStatus es = d2sFile.errorStatus();
        if (!(es & (D2SFile::ErrorStatus)D2SFile::ErrorStatusFlag::CHECKSUM)) {
            std::cout << "checksum is valid\n";
        }

        std::cout << std::hex << std::uppercase << d2sFile.checksum() << "\n";
        std::cout << std::hex << std::uppercase << d2sFile.computeChecksum() << "\n";
        d2sFile.fixChecksum();
        std::cout << std::hex << std::uppercase << d2sFile.checksum() << "\n";

        // uncomment this line to write newly computed checksum to the file
        // d2sFile.save();
    }
    catch (std::exception const& except) {
        std::cerr << except.what() << "\n";
    }

    return EXIT_SUCCESS;
}
