
#include "MicroProcessor.h"
#include "Register.h"

namespace tones {
namespace cpu {

/* ArithmeticAndLogicUnit */

ArithmeticAndLogicUnit::ArithmeticAndLogicUnit(tones::MicroProcessor &cpu)
    : _reg_A(cpu._reg_A)
    , _reg_DBB(cpu._reg_DBB)
    , _reg_P(cpu._reg_P)
{

}

ArithmeticAndLogicUnit::~ArithmeticAndLogicUnit()
{

}

/* One operand operations */

void ArithmeticAndLogicUnit::INC()
{
    ++_reg_A;
    checkZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::DEC()
{
    --_reg_A;
    checkZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::ASL()
{
    checkCarry(_reg_A & 0x80);
    _reg_A <<= 1;
    checkZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::LSR()
{
    checkCarry(_reg_A & 0x01);
    _reg_A >>= 1;
    checkZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::ROL()
{
    checkCarry(_reg_A & 0x80);
    _reg_A <<=1;
    _reg_A |= GET_BIT(_reg_P, StatusBit::C) ? 0x01 : 0x00;
    checkZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::ROR()
{
    checkCarry(_reg_A & 0x01); // TODO
    _reg_A >>= 1;
    _reg_A |= GET_BIT(_reg_P, StatusBit::C) ? 0x80 : 0x00;
    checkZeroNegative(_reg_A);
}

/* Two operands operations */

void ArithmeticAndLogicUnit::ORA()
{
    _reg_A |= _reg_DBB;
    checkZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::AND()
{
    _reg_A &= _reg_DBB;
    checkZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::EOR()
{
    _reg_A ^= _reg_DBB;
    checkZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::ADC()
{
    _reg_A += _reg_DBB; // TODO: Decimal ???
    // TODO: check status
}

void ArithmeticAndLogicUnit::SBC()
{

    _reg_A -= _reg_DBB; // TODO: Decimal ???
    // TODO: check status
}

void ArithmeticAndLogicUnit::CMP()
{
    _reg_A -= _reg_DBB;
    checkCarry(_reg_A & 0x80); // TODO
    checkZeroNegative(_reg_A);
}

inline void ArithmeticAndLogicUnit::checkZero(uint8_t &reg)
{
    SET_BIT(_reg_P, StatusBit::Z, !reg);
}

inline void ArithmeticAndLogicUnit::checkNegative(uint8_t &reg)
{
    SET_BIT(_reg_P, StatusBit::N, reg & 0x80);
}

inline void ArithmeticAndLogicUnit::checkZeroNegative(uint8_t &reg)
{
    checkZero(reg);
    checkNegative(reg);
}

inline void ArithmeticAndLogicUnit::checkCarry(uint8_t reg)
{
    SET_BIT(_reg_P, StatusBit::C, reg);
}

inline void ArithmeticAndLogicUnit::checkOverflow(uint8_t reg)
{
    SET_BIT(_reg_P, StatusBit::V, reg);
}

} // namespace cpu

/* MicroProcessor */

const std::array<MicroProcessor::Fetcher, cpu::AddressingModeSize>
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
    : Tickable(1)
    , _decoder(*this)
    , _alu(*this)
    , _bus(bus)
{
}

MicroProcessor::~MicroProcessor() {}

void MicroProcessor::reset()
{
    _reg_A = 0;
    _reg_X = 0;
    _reg_Y = 0;
    _reg_S = cpu::DefaultStack;
    _reg_P = cpu::DefaultStatus;

    // Load PC from reset vector
    _reg_PC = cpu::ResetVector;
    fetchTwo(*this);
    reg::mergeTwoBytes(_reg_PC, _reg_DBB, _reg_DL);
}

void MicroProcessor::dump(Registers_t &registers) const
{
    registers.A = _reg_A;
    registers.X = _reg_X;
    registers.Y = _reg_Y;
    registers.S = _reg_S;
    registers.P = _reg_P;
    registers.PC = _reg_PC;
}

void MicroProcessor::_tick()
{
    if (_skip) {
        --_skip;
        return;
    }

    // Fetch opration code
    _reg_AB = _reg_PC++;
    read();
    _reg_IR = _reg_DBB;

    _decoder.decode();

    // Fetch oprands
    _fetchers[_decoder.mode()](*this);

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
    reg::mergeTwoBytes(cpu._reg_AB, cpu._reg_DBB, cpu._reg_DL);
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

    reg::mergeTwoBytes(cpu._reg_AB, cpu._reg_DBB, cpu._reg_DL);
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

    reg::mergeTwoBytes(cpu._reg_AB, cpu._reg_DBB, cpu._reg_DL);
    cpu._reg_AB += cpu._reg_Y;
}

void MicroProcessor::fetchAbsoluteIndirect(MicroProcessor &cpu)
{
    fetchTwo(cpu);

    // Fetch ABL
    reg::mergeTwoBytes(cpu._reg_AB, cpu._reg_DBB, cpu._reg_DL);
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    // Fetch ABH
    ++cpu._reg_AB;
    cpu.read();

    reg::mergeTwoBytes(cpu._reg_AB, cpu._reg_DBB, cpu._reg_DL);
}

} // namespace tones
