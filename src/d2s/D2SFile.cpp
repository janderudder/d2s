#include "d2s/D2SFile.hpp"

#include <fstream>

using namespace std::filesystem;



////////////////////////////////////////////////////////////////////////////////
// utility
////////////////////////////////////////////////////////////////////////////////
static bool _is_file(path filePath)
{
    return is_regular_file(filePath) || is_symlink(filePath);
}



static uint32_t _rotateLeft(uint32_t const value)
{
    return (value << 1) | (value >> 31);
}



////////////////////////////////////////////////////////////////////////////////
// ctor
////////////////////////////////////////////////////////////////////////////////
D2SFile::D2SFile(std::filesystem::path filePath):
    m_path {filePath}
{
    if (!exists(filePath)) {
        throw std::runtime_error("cannot find or access file");
    }
    if (!is_regular_file(filePath) && !is_symlink(filePath)) {
        throw std::runtime_error("path does not refer to a file");
    }

    std::ifstream file {filePath, std::ios::binary};

    m_size = std::filesystem::file_size(filePath);
    m_content.resize(m_size);

    file.read((char*)m_content.data(), m_size);
}



////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////
auto D2SFile::size() const -> std::size_t
{
    return m_size;
}



auto D2SFile::checksum() const -> std::uint32_t
{
    return this->longAt(D2SFile::CHECKSUM_OFFSET);
}



auto D2SFile::content() const -> std::span<std::uint8_t const>
{
    return {m_content.cbegin(), m_content.size()};
}



auto D2SFile::longAt(std::size_t offset) const -> std::uint32_t
{
    return (m_content[offset+3] << 24)
        |  (m_content[offset+2] << 16)
        |  (m_content[offset+1] <<  8)
        |   m_content[offset];
}



auto D2SFile::shortAt(std::size_t offset) const -> std::uint16_t
{
    return (m_content[offset+1] << 8) | m_content[offset];
}



////////////////////////////////////////////////////////////////////////////////
// computation
////////////////////////////////////////////////////////////////////////////////
auto D2SFile::validate() const -> ErrorStatus
{
    ErrorStatus status = (ErrorStatus)ErrorStatusFlag::NONE;
    if (this->size() < D2SFile::MINIMUM_SIZE) {
        status |= (ErrorStatus)ErrorStatusFlag::SIZE;
    }
    if (this->longAt(0) != D2SFile::D2S_SIGNATURE) {
        status |= (ErrorStatus)ErrorStatusFlag::SIGNATURE;
    }
    if (this->longAt(D2SFile::CHECKSUM_OFFSET) != this->computeChecksum()) {
        status |= (ErrorStatus)ErrorStatusFlag::CHECKSUM;
    }
    return status;
}



auto D2SFile::computeChecksum() const -> std::uint32_t
{
    std::uint32_t sum = 0;
    std::vector<std::uint8_t> copy {m_content};
    *((uint32_t*)&copy[D2SFile::CHECKSUM_OFFSET]) = 0;
    for (std::uint8_t const byte : copy) {
        sum = (sum << 1) | (sum >> 31);
        sum += byte;
    }
    return sum;
}



////////////////////////////////////////////////////////////////////////////////
// setters
////////////////////////////////////////////////////////////////////////////////
void D2SFile::fixChecksum()
{

}



void D2SFile::setLongAt(std::uint32_t value, std::size_t offset)
{
    *(uint32_t*)&m_content[offset] = value;
}



void D2SFile::setShortAt(std::uint16_t value, std::size_t offset)
{
    *(uint16_t*)&m_content[offset] = value;
}



void D2SFile::setBytesAt(std::initializer_list<std::uint8_t> bytes, std::size_t offset)
{
    for (std::size_t idx=0; idx<bytes.size(); ++idx) {
        m_content[offset+idx] = *(bytes.begin()+idx);
    }
}



void D2SFile::save()
{
    std::ofstream file {m_path, std::ios::binary|std::ios::trunc};
    file.write((char*)m_content.data(), m_size);
}
