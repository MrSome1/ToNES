
#include "MicroProcessor.h"
#include "Register.h"

namespace tones {
namespace cpu {

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
    // TODO: check status
}

void ArithmeticAndLogicUnit::DEC()
{
    --_reg_A;
    // TODO: check status
}

void ArithmeticAndLogicUnit::ASL()
{
    _reg_A <<= 1;
    // TODO: check status
}

void ArithmeticAndLogicUnit::LSR()
{
    _reg_A >>= 1;
    // TODO: check status
}

void ArithmeticAndLogicUnit::ROL()
{

    // TODO: check status
}

void ArithmeticAndLogicUnit::ROR()
{

    // TODO: check status
}

/* Two operands operations */

void ArithmeticAndLogicUnit::ORA()
{
    _reg_A |= _reg_DBB;
    checkZero(_reg_A);
    checkNegative(_reg_A);
}

void ArithmeticAndLogicUnit::AND()
{
    _reg_A &= _reg_DBB;
    checkZero(_reg_A);
    checkNegative(_reg_A);
}

void ArithmeticAndLogicUnit::EOR()
{
    _reg_A ^= _reg_DBB;
    checkZero(_reg_A);
    checkNegative(_reg_A);
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
    checkCarry(_reg_A);
    checkZero(_reg_A);
    checkNegative(_reg_A);
}

inline void ArithmeticAndLogicUnit::checkCarry(uint8_t &reg)
{
    SET_BIT(_reg_P, StatusBit::Z, reg & 0x0100); // TODO
}

inline void ArithmeticAndLogicUnit::checkZero(uint8_t &reg)
{
    SET_BIT(_reg_P, StatusBit::Z, !reg);
}

inline void ArithmeticAndLogicUnit::checkOverflow(uint8_t &reg)
{
    SET_BIT(_reg_P, StatusBit::V, reg & 0x0100); // TODO
}

inline void ArithmeticAndLogicUnit::checkNegative(uint8_t &reg)
{
    SET_BIT(_reg_P, StatusBit::N, reg & 0x8000);
}

} // namespace cpu

/* MicroProcessor */

const std::array<MicroProcessor::Fetcher, cpu::code::Invalid + 1>
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
    fetchNull, // for addressing mode Invalid
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