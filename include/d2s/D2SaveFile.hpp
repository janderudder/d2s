#ifndef D2SFILE_HPP_INCLUDED
#define D2SFILE_HPP_INCLUDED

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <vector>
#include <span>


class D2SaveFile
{
    constexpr static std::size_t   MINIMUM_SIZE    = 0x64;
    constexpr static std::size_t   CHECKSUM_OFFSET = 0xC;
    constexpr static std::uint32_t SIGNATURE       = 0xAA55AA55;

    std::filesystem::path     m_path;
    std::size_t               m_size;
    std::vector<std::uint8_t> m_content;

public:
    D2SaveFile(std::filesystem::path filePath);

    auto size() const -> std::size_t;
    auto checksum() const -> std::uint32_t;
    auto content() const -> std::span<std::uint8_t const>;
    auto longAt(std::size_t offset) const -> std::uint32_t;
    auto shortAt(std::size_t offset) const -> std::uint16_t;

    auto computeChecksum() const -> std::uint32_t;

    auto hasValidSize() const -> bool;
    auto hasValidSignature() const -> bool;
    auto hasValidChecksum() const -> bool;

    void setLongAt(std::uint32_t value, std::size_t offset);
    void setShortAt(std::uint16_t value, std::size_t offset);
    void setBytesAt(std::initializer_list<std::uint8_t> bytes, std::size_t offset);
    void fixChecksum();
    void save();
};


#endif // SAVEFILE_HPP_INCLUDED
