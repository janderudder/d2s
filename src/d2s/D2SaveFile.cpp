#include "d2s/D2SaveFile.hpp"

#include <fstream>

#include "d2s/Quest.hpp"

namespace fs = std::filesystem;


////////////////////////////////////////////////////////////////////////////////
// ctor
////////////////////////////////////////////////////////////////////////////////
D2SaveFile::D2SaveFile(std::filesystem::path filePath):
    m_path {filePath}
{
    auto const fileStatus = fs::status(filePath);
    if (!fs::is_regular_file(fileStatus)) {
        throw std::runtime_error("Provided path does not refer to a file.");
    }
    std::ifstream file {filePath, std::ios::binary};

    m_size = std::filesystem::file_size(filePath);
    m_content.resize(m_size);

    file.read((char*)m_content.data(), m_size);
}



////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////
auto D2SaveFile::size() const -> std::size_t
{
    return m_size;
}



auto D2SaveFile::checksum() const -> std::uint32_t
{
    return this->longAt(D2SaveFile::CHECKSUM_OFFSET);
}



auto D2SaveFile::content() const -> std::span<std::uint8_t const>
{
    return {m_content.cbegin(), m_content.size()};
}



auto D2SaveFile::longAt(std::size_t offset) const -> std::uint32_t
{
    return (m_content[offset+3] << 24)
        |  (m_content[offset+2] << 16)
        |  (m_content[offset+1] <<  8)
        |   m_content[offset];
}



auto D2SaveFile::shortAt(std::size_t offset) const -> std::uint16_t
{
    return (m_content[offset+1] << 8) | m_content[offset];
}



////////////////////////////////////////////////////////////////////////////////
// computation
////////////////////////////////////////////////////////////////////////////////
auto D2SaveFile::computeChecksum() const -> std::uint32_t
{
    std::uint32_t sum = 0;

    auto const rotateLeft = [](std::uint32_t const sum) {
        return (sum << 1) | (sum >> 31);
    };

    // Several loops in order to avoid the addition of the current checksum.
    size_t byteIndex;
    for (byteIndex=0; byteIndex<D2SaveFile::CHECKSUM_OFFSET; ++byteIndex) {
        sum = m_content[byteIndex] + rotateLeft(sum);
    }
    // Still got to rotate though. It's as if we added a checksum of 0.
    for (size_t i=0; i<4; ++i) {
        sum = rotateLeft(sum);
    }
    for (byteIndex+=4; byteIndex<m_size; ++byteIndex) {
        sum = m_content[byteIndex] + rotateLeft(sum);
    }
    return sum;
}



////////////////////////////////////////////////////////////////////////////////
// validity
////////////////////////////////////////////////////////////////////////////////
auto D2SaveFile::hasValidSize() const -> bool
{
    return this->m_size >= D2SaveFile::MINIMUM_SIZE;
}



auto D2SaveFile::hasValidSignature() const -> bool
{
    return this->longAt(0) == D2SaveFile::SIGNATURE;
}



auto D2SaveFile::hasValidChecksum() const -> bool
{
    return this->longAt(D2SaveFile::CHECKSUM_OFFSET) == this->computeChecksum();
}



////////////////////////////////////////////////////////////////////////////////
// setters
////////////////////////////////////////////////////////////////////////////////
void D2SaveFile::setLongAt(std::uint32_t value, std::size_t offset)
{
    *(uint32_t*)&m_content[offset] = value;
}



void D2SaveFile::setShortAt(std::uint16_t value, std::size_t offset)
{
    *(uint16_t*)&m_content[offset] = value;
}



void D2SaveFile::setBytesAt(
    std::initializer_list<std::uint8_t> bytes,
    std::size_t offset
){
    for (std::size_t idx=0; idx<bytes.size(); ++idx) {
        m_content[offset+idx] = *(bytes.begin()+idx);
    }
}



void D2SaveFile::fixChecksum()
{
    setLongAt(this->computeChecksum(), D2SaveFile::CHECKSUM_OFFSET);
}



void D2SaveFile::save()
{
    std::ofstream file {m_path, std::ios::binary|std::ios::trunc};
    file.write((char*)m_content.data(), m_size);
}



////////////////////////////////////////////////////////////////////////////////
// private
////////////////////////////////////////////////////////////////////////////////
auto D2SaveFile::_serialize(Quest const &quest) const -> uint32_t
{
    return (quest.isCompleted())
        ? 0b1000'0000'0000'0000
        : static_cast<uint32_t>(quest.currentStateIndex());
}
