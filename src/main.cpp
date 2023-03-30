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

using namespace std::filesystem;

int main(int const argc, char const* const* argv)
{
    if (argc<2) {
        std::cerr << "no file provided\n";
        return EXIT_FAILURE;
    }

    D2SFile d2sFile {argv[1]};

    // d2sFile.setBytesAt({0xDD, 0xCC, 0xBB, 0xAA}, 0xC);
    d2sFile.setShortAt(0xBBAA, 0xC);
    std::cout << std::hex << std::uppercase << d2sFile.checksum() << "\n";

    d2sFile.setLongAt(0xDDCCBBAA, 0xC);
    std::cout << std::hex << std::uppercase << d2sFile.checksum() << "\n";

    std::cout << std::hex << std::uppercase << d2sFile.computeChecksum() << "\n";

    // d2sFile.save();

    return EXIT_SUCCESS;
}
