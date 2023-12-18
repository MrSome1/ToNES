
#include "MotherBoard.h"

namespace tones {

/* RandomAccessMemory */

RandomAccessMemory::RandomAccessMemory()
    : Device(RamBase, RamSize)
    , _memory(0x0800, 0) {}

void RandomAccessMemory::read(uint16_t address, uint8_t &buffer) const
{
    buffer = _memory[address & RamMask];
}

void RandomAccessMemory::write(uint16_t address, uint8_t data)
{
    _memory[address & RamMask] = data;
}

/* MotherBoard */

MotherBoard::MotherBoard() 
    : _cpu(_bus)
{

}

} // namespace tones