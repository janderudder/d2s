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

#include "d2s/D2SaveFile.hpp"


int sample_D2SaveFile(int const argc, char const* const* argv)
{
    if (argc<2) {
        std::cerr << "Please provide a file path as argument.\n";
        return EXIT_FAILURE;
    }

    try {
        D2SaveFile d2sFile {argv[1]};

        // get information about validity of the file as a D2 save file
        if (!d2sFile.hasValidSize()) {
            throw std::invalid_argument("File is too small.");
        }
        else if (!d2sFile.hasValidSignature()) {
            throw std::invalid_argument("File has invalid signature.");
        }

        std::cout << "File looks like a D2S file.\n";
        std::cout << "Checksum is " << (!d2sFile.hasValidChecksum()?"in":"") << "valid.\n";

        auto const currentChecksum = d2sFile.checksum();
        auto const computedChecksum = d2sFile.computeChecksum();

        // display current checksum (may be good or corrupt)
        std::cout << "Current checksum:  " << std::hex << std::uppercase << currentChecksum << "\n";

        // compute the correct checksum (may be the same than above)
        std::cout << "Computed checksum: " << std::hex << std::uppercase << computedChecksum << "\n";

        if (currentChecksum != computedChecksum)
        {
            // compute and write the checksum to the data structure
            d2sFile.fixChecksum();

            // read the checksum again from the structure (may have changed with the fix)
            std::cout << "Fixed checksum:    " << std::hex << std::uppercase << d2sFile.checksum() << "\n";

            // uncomment the following line to write data to the file (with correct checksum)
            // d2sFile.save();
        }
    }
    catch (std::invalid_argument const& except) {
        std::cout << except.what() << " Probably not a D2S file.\n";
    }
    catch (std::exception const& except) {
        std::cout << except.what() << "\n";
    }

    return EXIT_SUCCESS;
}
