
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

/* ArithmeticAndLogicUnit */

ArithmeticAndLogicUnit::ArithmeticAndLogicUnit(tones::MicroProcessor &cpu)
    : _cpu(cpu)
{

}

ArithmeticAndLogicUnit::~ArithmeticAndLogicUnit()
{

}

void ArithmeticAndLogicUnit::ORA()
{
    _reg = _cpu._reg_DBB;
    _reg |= _cpu._reg_A;
    checkZero();
    checkNegative();
    _cpu._reg_A = _reg;
}

void ArithmeticAndLogicUnit::AND()
{
    _reg = _cpu._reg_DBB;
    _reg &= _cpu._reg_A;
    checkZero();
    checkNegative();
    _cpu._reg_A = _reg;
}

void ArithmeticAndLogicUnit::EOR()
{
    _reg = _cpu._reg_DBB;
    _reg ^= _cpu._reg_A;
    checkZero();
    checkNegative();
    _cpu._reg_A = _reg;
}

void ArithmeticAndLogicUnit::ADC()
{
    // TODO: Decimal ???
    _reg = _cpu._reg_DBB;
    _reg += _cpu._reg_A;
    // TODO: check status
    _cpu._reg_A = _reg;
}

void ArithmeticAndLogicUnit::SBC()
{
    // TODO: Decimal ???
    _reg = _cpu._reg_A;
    _reg -= _cpu._reg_DBB;
    // TODO: check status
    _cpu._reg_A = _reg;
}

void ArithmeticAndLogicUnit::CMP()
{
    _reg = _cpu._reg_A;
    _reg -= _cpu._reg_DBB;
    checkCarry();
    checkZero();
    checkNegative();
    _cpu._reg_A = _reg;
}

inline void ArithmeticAndLogicUnit::checkCarry()
{
    _cpu._reg_P.set(StatusBit::Zero, _reg & 0x0100); // ???
}

inline void ArithmeticAndLogicUnit::checkZero()
{
    _cpu._reg_P.set(StatusBit::Zero, !_reg);
}

inline void ArithmeticAndLogicUnit::checkOverflow()
{
    _cpu._reg_P.set(StatusBit::Overflow, _reg & 0x0100); // ???
}

inline void ArithmeticAndLogicUnit::checkNegative()
{
    _cpu._reg_P.set(StatusBit::Negative, _reg & 0x8000);
}

} // namespace cpu

/* MicroProcessor */

std::array<std::function<void(MicroProcessor&)>, cpu::AddressingModeCount>
MicroProcessor::_fetchers = {
    fetchAccumulator,
    fetchImmediate,
    fetchAbsolute,
    fetchZeroPage,
    fetchIndexedZeroPageX,
    fetchIndexedZeroPageY,
    fetchIndexedAbsoluteX,
    fetchIndexedAbsoluteY,
    fetchImplied,
    fetchRelative,
    fetchIndexedIndirect,
    fetchIndirectIndexed,
    fetchAbsoluteIndirect
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
    int mode = static_cast<int>(_decoder.decode());
    _fetchers[mode](*this);

    // Execute
    _decoder.execute();
}

/*inline*/ void MicroProcessor::read()
{
    _bus.read(_reg_AB, _reg_DBB);
}

/*inline*/ void MicroProcessor::write()
{
    _bus.write(_reg_AB, _reg_DBB);
}

void MicroProcessor::fetchAccumulator(MicroProcessor &cpu)
{
    // Operand already in accumulator
}

void MicroProcessor::fetchImmediate(MicroProcessor &cpu)
{
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();
}

void MicroProcessor::fetchAbsolute(MicroProcessor &cpu)
{
    // Fetch ABL
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    // Fetch ABH
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();

    // Fetch operand
    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
    //cpu.read();
}

void MicroProcessor::fetchZeroPage(MicroProcessor &cpu)
{
    // Fetch ABL
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();

    // Fetch operand
    cpu._reg_AB = cpu._reg_DBB;
    //cpu.read();
}

void MicroProcessor::fetchIndexedZeroPage(MicroProcessor &cpu, uint8_t index)
{
    // Fetch ABL
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();

    // Fetch operand
    cpu._reg_AB = (uint8_t)(cpu._reg_DBB + index);
    //cpu.read();
}

void MicroProcessor::fetchIndexedZeroPageX(MicroProcessor &cpu)
{
    fetchIndexedZeroPage(cpu, cpu._reg_X);
}

void MicroProcessor::fetchIndexedZeroPageY(MicroProcessor &cpu)
{
    fetchIndexedZeroPage(cpu, cpu._reg_Y);
}

void MicroProcessor::fetchIndexedAbsolute(MicroProcessor &cpu, uint8_t index)
{
    // Fetch ABL
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    // Fetch ABH
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();

    // Fetch operand
    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
    cpu._reg_AB += index;
    //cpu.read();
}

void MicroProcessor::fetchIndexedAbsoluteX(MicroProcessor &cpu)
{
    fetchIndexedAbsolute(cpu, cpu._reg_X);
}

void MicroProcessor::fetchIndexedAbsoluteY(MicroProcessor &cpu)
{
    fetchIndexedAbsolute(cpu, cpu._reg_Y);
}

void MicroProcessor::fetchImplied(MicroProcessor &cpu)
{
    // TODO
}

void MicroProcessor::fetchRelative(MicroProcessor &cpu)
{
    // Fetch branch offset
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();
    // TODO: What to do with the offset ??
}

void MicroProcessor::fetchIndexedIndirect(MicroProcessor &cpu)
{
    // Fetch indirect ABL
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();

    // Fetch ABL
    cpu._reg_AB = (uint8_t)(cpu._reg_DBB + cpu._reg_X);
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    // Fetch ABH
    ++cpu._reg_AB;
    cpu.read();

    // Fetch operand
    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
    //cpu.read();
}

void MicroProcessor::fetchIndirectIndexed(MicroProcessor &cpu)
{
    // Fetch indirect ABL
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();

    // Fetch ABL
    cpu._reg_AB = cpu._reg_DBB;
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    // Fetch ABH
    ++cpu._reg_AB;
    cpu.read();

    // Fetch operand
    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
    cpu._reg_AB += cpu._reg_Y;
    //cpu.read();
}

void MicroProcessor::fetchAbsoluteIndirect(MicroProcessor &cpu)
{
    // Fetch indirect ABL
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    // Fetch indirect ABH
    cpu._reg_AB = cpu._reg_PC++;
    cpu.read();

    // Fetch ABL
    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
    cpu.read();

    cpu._reg_DL = cpu._reg_DBB;

    // Fetch ABH
    ++cpu._reg_AB;
    cpu.read();

    // Fetch operand
    cpu.setAB(cpu._reg_DBB, cpu._reg_DL);
    //cpu.read();
}

inline void MicroProcessor::setABL(uint8_t val)
{
    _reg_AB &= 0xff00;
    _reg_AB |= (uint16_t)val;
}

inline void MicroProcessor::setABH(uint8_t val)
{
    _reg_AB &= 0xff00;
    _reg_AB |= (uint16_t)val << 8;
}

inline void MicroProcessor::setAB(uint8_t abh, uint8_t abl)
{
    _reg_AB = (uint16_t)abh << 8 | (uint16_t)abl;
}

} // namespace tones