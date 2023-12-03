
#include "MicroProcessor.h"

namespace ToNES {

StatusRegister::StatusRegister()
{
    reset();
}

void StatusRegister::reset()
{
    status = 0b00100100;
}

void StatusRegister::set(StatusBit bit, bool value)
{
    uint8_t mask = 0x01 << static_cast<int>(bit);    
    if (value) {
        status |= mask;
    } else {
        status &= ~mask;
    }
}

bool StatusRegister::get(StatusBit bit)
{
    uint8_t mask = 0x01 << static_cast<int>(bit);    
    return status & mask;
}

MicroProcessor::MicroProcessor()
{

}

 MicroProcessor::~MicroProcessor()
 {

 }

} // namespace ToNES