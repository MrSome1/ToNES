
#include "Device.h"

namespace tones {

/* RandomAccessMemory */

bool RandomAccessMemory::contains(uint16_t addr) const
{
    return addr < RamUpperBound;
}

void RandomAccessMemory::read(uint16_t address, uint8_t &buffer) const
{
    buffer = _memory[address & RamMask];
}

void RandomAccessMemory::write(uint16_t address, uint8_t data)
{
    _memory[address & RamMask] = data;
}

/* ReadOnlyMemory */

ReadOnlyMemory::ReadOnlyMemory(const std::vector<uint8_t> &memory)
    : _memory(memory) {}

bool ReadOnlyMemory::contains(uint16_t addr) const
{
    return addr >= RomLowerBankBase;
}

void ReadOnlyMemory::read(uint16_t address, uint8_t &buffer) const
{
    buffer = _memory[address & RomMask];
}

void ReadOnlyMemory::write(uint16_t address, uint8_t data)
{
    /* Just do nothing at all*/
}

/* VideoRandomAccessMemory */

bool VideoRandomAccessMemory::contains(uint16_t addr) const
{
    return addr >= VramLowerBound && addr < VramUpperBound;
}

void VideoRandomAccessMemory::read(uint16_t address, uint8_t &buffer) const
{
    buffer = _memory[address & VramMask];
}

void VideoRandomAccessMemory::write(uint16_t address, uint8_t data)
{
    _memory[address & VramMask] = data;
}

/* PatternTalbe */

PatternTables::PatternTables(const std::vector<uint8_t> &memory)
    : _memory(memory) {}

bool PatternTables::contains(uint16_t addr) const
{
    return addr < TotalSize;
}

void PatternTables::read(uint16_t address, uint8_t &buffer) const
{
    buffer = _memory[address];
}

void PatternTables::write(uint16_t address, uint8_t data)
{
    /* TODO */
}

} // namespace tones