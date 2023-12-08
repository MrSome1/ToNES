
#include "MicroProcessor.h"

namespace tones {
namespace cpu {

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

ArithmeticAndLogicUnit::ArithmeticAndLogicUnit(tones::MicroProcessor &cpu)
    : _cpu(cpu)
{

}

ArithmeticAndLogicUnit::~ArithmeticAndLogicUnit()
{

}

} // namespace cpu

MicroProcessor::MicroProcessor(DataBus &bus)
    : _decoder(*this)
    , _alu(*this)
    , _bus(bus)
{

}

MicroProcessor::~MicroProcessor()
{

}

void MicroProcessor::step()
{
    // TODO: compile rrror
    //_reg_I = _bus.read(_reg_PC++);
}

} // namespace tones