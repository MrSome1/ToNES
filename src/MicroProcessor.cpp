
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
    value = DefaultStatus;
}

void StatusRegister::set(StatusBit bit, bool val)
{
    uint8_t mask = 0x01 << static_cast<int>(bit);    
    val ? value |= mask : value &= ~mask;
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

/* One operand operations */

void ArithmeticAndLogicUnit::INC()
{
    ++_reg_A;
}

void ArithmeticAndLogicUnit::DEC()
{
    --_reg_A;
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

MicroProcessor::MicroProcessor(Bus &bus)
    : _decoder(*this)
    , _alu(*this)
    , _bus(bus) {}

MicroProcessor::~MicroProcessor() {}

void MicroProcessor::reset()
{
    _reg_A = 0;
    _reg_X = 0;
    _reg_Y = 0;
    _reg_S = 0xfd; // TODO: Where is this mentioned
    _reg_P.reset();

    // Load PC from reset vector
    _reg_PC = cpu::ResetVector;
    fetchTwo(*this);
    setAddress(_reg_PC, _reg_DBB, _reg_DL);
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

void MicroProcessor::dump(Registers_t &registers) const
{
    registers.A = _reg_A;
    registers.X = _reg_X;
    registers.Y = _reg_Y;
    registers.S = _reg_S;
    registers.P = _reg_P.value;
    registers.PC = _reg_PC;
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
    cpu.setAddress(cpu._reg_AB, cpu._reg_DBB, cpu._reg_DL);
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

    cpu.setAddress(cpu._reg_AB, cpu._reg_DBB, cpu._reg_DL);
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

    cpu.setAddress(cpu._reg_AB, cpu._reg_DBB, cpu._reg_DL);
    cpu._reg_AB += cpu._reg_Y;
}

void MicroProcessor::fetchAbsoluteIndirect(MicroProcessor &cpu)
{
    fetchTwo(cpu);

    // Fetch ABL
    cpu.setAddress(cpu._reg_AB, cpu._reg_DBB, cpu._reg_DL);
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    // Fetch ABH
    ++cpu._reg_AB;
    cpu.read();

    cpu.setAddress(cpu._reg_AB, cpu._reg_DBB, cpu._reg_DL);
}

inline void MicroProcessor::setAddress(uint16_t &reg, uint8_t msb, uint8_t lsb)
{
    reg = (uint16_t)msb << 8 | (uint16_t)lsb;
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
    cpu.setAddress(cpu._reg_AB, cpu._reg_DBB, cpu._reg_DL);
    cpu._reg_AB += index;
}

} // namespace tones