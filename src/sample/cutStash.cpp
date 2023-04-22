#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "d2s/D2File.hpp"


int cutStash(int const argc, char const* const* argv)
{
    if (argc<4) {
        std::cerr << "Please provide three file paths as argument (one input and two output).\n";
        return EXIT_FAILURE;
    }

    try {
        D2File const d2File {argv[1]};

        if (!d2File.hasValidSignature()) {
            throw std::invalid_argument("File has invalid signature.");
        }

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
         * data is read, now save files
         */
        bool overwrite = false;

        using namespace std::string_view_literals;

        for (ssize_t idx=0; idx<argc; ++idx) {
            if ("-w"sv == argv[idx]) {
                overwrite = true;
            }
        }

        namespace fs = std::filesystem;

        if (!overwrite && (fs::exists(argv[2]) || fs::exists(argv[3]))) {
            std::cout << "One or more output files already exists. Aborting\n";
            return EXIT_FAILURE;
        }

        std::ofstream out1 {argv[2], std::ios::binary};
        std::ofstream out2 {argv[3], std::ios::binary};

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
        std::cout << "written "<< stashPages.size() <<" pages to disk.\n";
        return EXIT_SUCCESS;
    }

    catch (std::invalid_argument const& except) {
        std::cout << except.what() << " Probably not a d2i file.\n";
    }

    catch (std::exception const& except) {
        std::cout << except.what() << "\n";
    }

    return EXIT_FAILURE;
}
