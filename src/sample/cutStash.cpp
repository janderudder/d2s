#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "d2s/D2File.hpp"


int cutStash(int const argc, char const* const* argv)
{
    /**
     * parse program arguments
     */
    std::vector<std::string_view> positional;
    bool overwrite = false;

    using namespace std::string_view_literals;

    for (ssize_t idx=1; idx<argc; ++idx)
    {
        std::string_view const arg = argv[idx];

        if (arg[0] == '-') {
            if ("-w"sv == arg) {
                overwrite = true;
            }
        }
        else if (positional.size() < 3) {
            positional.push_back(arg);
        }
    }

    if (positional.size() < 3) {
        std::cerr << "Please provide three file paths as argument (the first for input, then two for output).\n";
        return EXIT_FAILURE;
    }

    std::string_view const
        inPath = positional[0],
        outPath1 = positional[1],
        outPath2 = positional[2];

    try {
        /**
         * open input file
         */

        D2File const d2File {inPath};

        if (!d2File.hasValidSignature()) {
            throw std::invalid_argument("file has invalid signature");
        }

        /**
         * open output file(s)
         */
        namespace fs = std::filesystem;

        if (!overwrite && (fs::exists(outPath1) || fs::exists(outPath2))) {
            std::cerr << "One or more output files already exists. Aborting\n";
            return EXIT_FAILURE;
        }

        std::ofstream out1 {outPath1.data(), std::ios::binary};
        std::ofstream out2 {outPath2.data(), std::ios::binary};

        /**
         * read d2i data into separate pages
         */

        auto const data = d2File.content();

        std::vector<std::vector<uint8_t>> stashPages;

        size_t start = 0;

        for (size_t idx=4; idx<d2File.size(); ++idx)
        {
            if (d2File.longAt(idx) == 0xAA55AA55)
            {
                auto const beg = data.begin() + start;
                auto const end = data.begin() + idx;
                stashPages.push_back({beg, end});
                start = idx;
                idx += 3; // next loop iteration will make it +4
            }
        }

        stashPages.emplace_back(data.begin() + start, data.begin()+d2File.size());

        /**
         * save to output files
         */

        for (size_t pageIndex=0; pageIndex<3 && pageIndex<stashPages.size(); ++pageIndex)
        {
            auto const& stashPage = stashPages[pageIndex];

            for (size_t byteIndex=0; byteIndex<stashPage.size(); ++byteIndex) {
                out1 << stashPage[byteIndex];
            }
        }

        for (size_t pageIndex=3; pageIndex<6 && pageIndex<stashPages.size(); ++pageIndex)
        {
            auto const& stashPage = stashPages[pageIndex];

            for (size_t byteIndex=0; byteIndex<stashPage.size(); ++byteIndex) {
                out2 << stashPage[byteIndex];
            }
        }

        out1.close();
        out2.close();

        std::cout << "Written "<< stashPages.size() <<" pages to disk.\n";
        return EXIT_SUCCESS;
    }

    catch (std::invalid_argument const& except) {
        std::cerr << "Error: " << except.what() << ". Probably not a d2i file.\n";
    }
    catch (std::exception const& except) {
        std::cerr << "Exception: " << except.what() << "\n";
    }
    catch (...) {
        std::cerr << "Unknown exception\n";
    }

    return EXIT_FAILURE;
}
