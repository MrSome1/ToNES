
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
    SET_BIT(_reg_P, StatusBit::Z, _reg & 0x0100); // TODO
}

inline void ArithmeticAndLogicUnit::checkZero()
{
    SET_BIT(_reg_P, StatusBit::Z, !_reg);
}

inline void ArithmeticAndLogicUnit::checkOverflow()
{
    SET_BIT(_reg_P, StatusBit::O, _reg & 0x0100); // TODO
}

inline void ArithmeticAndLogicUnit::checkNegative()
{
    SET_BIT(_reg_P, StatusBit::N, _reg & 0x8000);
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