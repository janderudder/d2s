#ifndef D2SFILE_HPP_INCLUDED
#define D2SFILE_HPP_INCLUDED

#include "d2s/D2File.hpp"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <vector>
#include <span>

class Quest;


class D2SaveFile : public D2File
{
    constexpr static std::size_t   MINIMUM_SIZE    = 0x64;
    constexpr static std::size_t   CHECKSUM_OFFSET = 0xC;

public:
    D2SaveFile(std::filesystem::path filePath);

    auto checksum() const -> std::uint32_t;
    auto computeChecksum() const -> std::uint32_t;

    auto hasValidSize() const -> bool;
    auto hasValidChecksum() const -> bool;

    void fixChecksum();

private:
    auto _serialize(Quest const& quest) const -> uint32_t;

};


#endif // SAVEFILE_HPP_INCLUDED
