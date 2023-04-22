#include "d2s/D2File.hpp"

#include <fstream>

namespace fs = std::filesystem;


D2File::D2File(std::filesystem::path filePath):
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
auto D2File::size() const -> size_t
{
    return m_size;
}


auto D2File::content() const -> std::span<uint8_t const>
{
    return {m_content.cbegin(), m_content.size()};
}



auto D2File::longAt(size_t offset) const -> uint32_t
{
    return (m_content[offset+3] << 24)
        |  (m_content[offset+2] << 16)
        |  (m_content[offset+1] <<  8)
        |   m_content[offset];
}



auto D2File::shortAt(size_t offset) const -> uint16_t
{
    return (m_content[offset+1] << 8) | m_content[offset];
}



////////////////////////////////////////////////////////////////////////////////
// validity
////////////////////////////////////////////////////////////////////////////////
auto D2File::hasValidSignature() const -> bool
{
    return this->longAt(0) == D2File::SIGNATURE;
}



////////////////////////////////////////////////////////////////////////////////
// setters
////////////////////////////////////////////////////////////////////////////////
void D2File::setLongAt(uint32_t value, size_t offset)
{
    *(uint32_t*)&m_content[offset] = value;
}



void D2File::setShortAt(uint16_t value, size_t offset)
{
    *(uint16_t*)&m_content[offset] = value;
}



void D2File::setBytesAt(
    std::initializer_list<uint8_t> bytes,
    size_t offset
){
    for (size_t idx=0; idx<bytes.size(); ++idx) {
        m_content[offset+idx] = *(bytes.begin()+idx);
    }
}



void D2File::save()
{
    std::ofstream file {m_path, std::ios::binary|std::ios::trunc};
    file.write((char*)m_content.data(), m_size);
}
