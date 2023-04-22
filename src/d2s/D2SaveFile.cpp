#include "d2s/D2SaveFile.hpp"

#include <fstream>

#include "d2s/Quest.hpp"

namespace fs = std::filesystem;


////////////////////////////////////////////////////////////////////////////////
// ctor
////////////////////////////////////////////////////////////////////////////////
D2SaveFile::D2SaveFile(std::filesystem::path filePath):
    D2File {std::move(filePath)}
{
}



////////////////////////////////////////////////////////////////////////////////
// getters
////////////////////////////////////////////////////////////////////////////////
auto D2SaveFile::checksum() const -> uint32_t
{
    return this->longAt(D2SaveFile::CHECKSUM_OFFSET);
}



////////////////////////////////////////////////////////////////////////////////
// computation
////////////////////////////////////////////////////////////////////////////////
auto D2SaveFile::computeChecksum() const -> uint32_t
{
    uint32_t sum = 0;

    auto const rotateLeft = [](uint32_t const sum) {
        return (sum << 1) | (sum >> 31);
    };

    // Several loops in order to avoid the addition of the current checksum.
    size_t byteIndex;
    for (byteIndex=0; byteIndex<D2SaveFile::CHECKSUM_OFFSET; ++byteIndex) {
        sum = this->content()[byteIndex] + rotateLeft(sum);
    }
    // Still got to rotate though. It's as if we added a checksum of 0.
    for (size_t i=0; i<4; ++i) {
        sum = rotateLeft(sum);
    }
    for (byteIndex+=4; byteIndex<this->size(); ++byteIndex) {
        sum = this->content()[byteIndex] + rotateLeft(sum);
    }
    return sum;
}



////////////////////////////////////////////////////////////////////////////////
// validity
////////////////////////////////////////////////////////////////////////////////
auto D2SaveFile::hasValidSize() const -> bool
{
    return this->size() >= D2SaveFile::MINIMUM_SIZE;
}



auto D2SaveFile::hasValidChecksum() const -> bool
{
    return this->longAt(D2SaveFile::CHECKSUM_OFFSET) == this->computeChecksum();
}



////////////////////////////////////////////////////////////////////////////////
// setters
////////////////////////////////////////////////////////////////////////////////
void D2SaveFile::fixChecksum()
{
    setLongAt(this->computeChecksum(), D2SaveFile::CHECKSUM_OFFSET);
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
