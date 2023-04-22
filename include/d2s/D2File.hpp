#ifndef D2FILE_HPP_INCLUDED
#define D2FILE_HPP_INCLUDED

#include <cstdint>
#include <filesystem>
#include <vector>
#include <span>

class D2File
{
    constexpr static uint32_t SIGNATURE = 0xAA55AA55;

    std::filesystem::path m_path;
    size_t                m_size;
    std::vector<uint8_t>  m_content;

public:
    D2File(std::filesystem::path filePath);

    auto size() const -> size_t;
    auto content() const -> std::span<uint8_t const>;
    auto longAt(size_t offset) const -> uint32_t;
    auto shortAt(size_t offset) const -> uint16_t;

    auto hasValidSignature() const -> bool;

    void setLongAt(uint32_t value, size_t offset);
    void setShortAt(uint16_t value, size_t offset);
    void setBytesAt(std::initializer_list<uint8_t> bytes, size_t offset);
    void save();
};


#endif // D2FILE_HPP_INCLUDED
