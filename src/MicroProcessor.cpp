
#include "MicroProcessor.h"

namespace tones {
namespace cpu {

/* StatusRegister */

StatusRegister::StatusRegister()
{
    reset();
}

void StatusRegister::reset()
{
    value = 0b00100100;
}

void StatusRegister::set(StatusBit bit, bool val)
{
    uint8_t mask = 0x01 << static_cast<int>(bit);    
    if (val) {
        value |= mask;
    } else {
        value &= ~mask;
    }
}

bool StatusRegister::get(StatusBit bit)
{
    uint8_t mask = 0x01 << static_cast<int>(bit);    
    return value & mask;
}

/* ArithmeticAndLogicUnit */

ArithmeticAndLogicUnit::ArithmeticAndLogicUnit(tones::MicroProcessor &cpu)
    : _reg_A(cpu._reg_A)
    , _reg_P(cpu._reg_P)
    , _reg_DBB(cpu._reg_DBB)
{

}

ArithmeticAndLogicUnit::~ArithmeticAndLogicUnit()
{

}

/* Two operands operations */

void ArithmeticAndLogicUnit::INC()
{

}

void ArithmeticAndLogicUnit::DEC()
{

}

void ArithmeticAndLogicUnit::ASL()
{

}

void ArithmeticAndLogicUnit::LSR()
{

}

void ArithmeticAndLogicUnit::ROL()
{

}

void ArithmeticAndLogicUnit::ROR()
{

}

/* Two operands operations */

void ArithmeticAndLogicUnit::LDA()
{
    _reg = _reg_DBB;
    checkZero();
    checkNegative();
    _reg_A = _reg;
}

void ArithmeticAndLogicUnit::ORA()
{
    _reg = _reg_DBB;
    _reg |= _reg_A;
    checkZero();
    checkNegative();
    _reg_A = _reg;
}

void ArithmeticAndLogicUnit::AND()
{
    _reg = _reg_DBB;
    _reg &= _reg_A;
    checkZero();
    checkNegative();
    _reg_A = _reg;
}

void ArithmeticAndLogicUnit::EOR()
{
    _reg = _reg_DBB;
    _reg ^= _reg_A;
    checkZero();
    checkNegative();
    _reg_A = _reg;
}

void ArithmeticAndLogicUnit::ADC()
{
    // TODO: Decimal ???
    _reg = _reg_DBB;
    _reg += _reg_A;
    // TODO: check status
    _reg_A = _reg;
}

void ArithmeticAndLogicUnit::SBC()
{
    // TODO: Decimal ???
    _reg = _reg_A;
    _reg -= _reg_DBB;
    // TODO: check status
    _reg_A = _reg;
}

void ArithmeticAndLogicUnit::CMP()
{
    _reg = _reg_A;
    _reg -= _reg_DBB;
    checkCarry();
    checkZero();
    checkNegative();
    _reg_A = _reg;
}

inline void ArithmeticAndLogicUnit::checkCarry()
{
    _reg_P.set(StatusBit::Zero, _reg & 0x0100); // ???
}

inline void ArithmeticAndLogicUnit::checkZero()
{
    _reg_P.set(StatusBit::Zero, !_reg);
}

inline void ArithmeticAndLogicUnit::checkOverflow()
{
    _reg_P.set(StatusBit::Overflow, _reg & 0x0100); // ???
}

inline void ArithmeticAndLogicUnit::checkNegative()
{
    _reg_P.set(StatusBit::Negative, _reg & 0x8000);
}

} // namespace cpu

/* MicroProcessor */

std::array<std::function<void(MicroProcessor&)>, cpu::AddressingModeCount>
MicroProcessor::_fetchers = {
    fetchNull, 
    fetchNull,
    fetchImmediate,
    fetchAbsolute,
    fetchZeroPage,
    fetchIndexedZeroPageX,
    fetchIndexedZeroPageY,
    fetchIndexedAbsoluteX,
    fetchIndexedAbsoluteY,
    fetchRelative,
    fetchIndexedIndirect,
    fetchIndirectIndexed,
    fetchAbsoluteIndirect,
};

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
    // Fetch opration code
    _reg_AB = _reg_PC++;
    read();
    _reg_IR = _reg_DBB;

    // Fetch oprands
    int mode = _decoder.decode();
    _fetchers[mode](*this);

    // Execute
    _decoder.load();
    _decoder.execute();
    _decoder.save();
}

void MicroProcessor::fetchNull(MicroProcessor &cpu) {
    /* No operand needed */
}

void MicroProcessor::fetchImmediate(MicroProcessor &cpu)
{
    fetchOne(cpu);
}

void MicroProcessor::fetchAbsolute(MicroProcessor &cpu)
{
    fetchTwo(cpu);
    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
}

void MicroProcessor::fetchZeroPage(MicroProcessor &cpu)
{
    fetchOne(cpu);
    cpu._reg_AB = cpu._reg_DBB;
}

void MicroProcessor::fetchIndexedZeroPageX(MicroProcessor &cpu)
{
    fetchIndexedZeroPage(cpu, cpu._reg_X);
}

void MicroProcessor::fetchIndexedZeroPageY(MicroProcessor &cpu)
{
    fetchIndexedZeroPage(cpu, cpu._reg_Y);
}

void MicroProcessor::fetchIndexedAbsoluteX(MicroProcessor &cpu)
{
    fetchIndexedAbsolute(cpu, cpu._reg_X);
}

void MicroProcessor::fetchIndexedAbsoluteY(MicroProcessor &cpu)
{
    fetchIndexedAbsolute(cpu, cpu._reg_Y);
}

void MicroProcessor::fetchRelative(MicroProcessor &cpu)
{
    fetchOne(cpu); // branch offset
}

void MicroProcessor::fetchIndexedIndirect(MicroProcessor &cpu)
{
    fetchOne(cpu);

    // Fetch ABL
    cpu._reg_AB = (uint8_t)(cpu._reg_DBB + cpu._reg_X);
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    // Fetch ABH
    ++cpu._reg_AB;
    cpu.read();

    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
}

void MicroProcessor::fetchIndirectIndexed(MicroProcessor &cpu)
{
    fetchOne(cpu);

    // Fetch ABL
    cpu._reg_AB = cpu._reg_DBB;
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    // Fetch ABH
    ++cpu._reg_AB;
    cpu.read();

    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
    cpu._reg_AB += cpu._reg_Y;
}

void MicroProcessor::fetchAbsoluteIndirect(MicroProcessor &cpu)
{
    fetchTwo(cpu);

    // Fetch ABL
    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    // Fetch ABH
    ++cpu._reg_AB;
    cpu.read();

    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
}

inline void MicroProcessor::setAB(uint8_t abh, uint8_t abl)
{
    _reg_AB = (uint16_t)abh << 8 | (uint16_t)abl;
}

inline void MicroProcessor::fetchOne(MicroProcessor &cpu)
{
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();
}

inline void MicroProcessor::fetchTwo(MicroProcessor &cpu)
{
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();
}

inline void MicroProcessor::fetchIndexedZeroPage(MicroProcessor &cpu, uint8_t index)
{
    fetchOne(cpu);
    cpu._reg_AB = (uint8_t)(cpu._reg_DBB + index);
}

inline void MicroProcessor::fetchIndexedAbsolute(MicroProcessor &cpu, uint8_t index)
{
    fetchTwo(cpu);
    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
    cpu._reg_AB += index;
}

} // namespace tones