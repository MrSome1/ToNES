
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

void ArithmeticAndLogicUnit::checkZero(uint8_t &reg)
{
    SET_BIT(_reg_P, StatusBit::Z, !reg);
}

void ArithmeticAndLogicUnit::checkNegative(uint8_t &reg)
{
    SET_BIT(_reg_P, StatusBit::N, reg & 0x80);
}

void ArithmeticAndLogicUnit::checkZeroNegative(uint8_t &reg)
{
    checkZero(reg);
    checkNegative(reg);
}

void ArithmeticAndLogicUnit::checkCarry(uint8_t reg)
{
    SET_BIT(_reg_P, StatusBit::C, reg);
}

void ArithmeticAndLogicUnit::checkOverflow(uint8_t reg)
{
    SET_BIT(_reg_P, StatusBit::V, reg);
}

/* InstructionDecoder */

InstructionDecoder::InstructionDecoder(tones::MicroProcessor &cpu)
    : _cpu(cpu)
    , _alu(cpu._alu)
    , _operation(&UnknownOperation)
{

}

inline void InstructionDecoder::decode()
{
    _operation = OperationSet[_cpu._reg_IR];
    _cpu._skip = _operation->cycles; // TODO: Dynamic cycles
}

inline void InstructionDecoder::execute()
{
    (this->*(_operation->type->executor))();
}

inline void InstructionDecoder::load()
{
    if (hasOperands(_operation) && needsToLoad(_operation))
        _cpu.read();
}

inline void InstructionDecoder::save()
{
    if (hasOperands(_operation) && needsToSave(_operation))
        _cpu.write();
}

/* Instructions */

void InstructionDecoder::ADC()
{
    _alu.ADC();
}

void InstructionDecoder::AND()
{
    _alu.AND();
}

void InstructionDecoder::ASL()
{
    _alu.ASL();
}

void InstructionDecoder::BCC()
{
    if (!GET_BIT(_cpu._reg_P, StatusBit::C))
        _cpu.branch();
}

void InstructionDecoder::BCS()
{
    if (GET_BIT(_cpu._reg_P, StatusBit::C))
        _cpu.branch();
}

void InstructionDecoder::BEQ()
{
    if (GET_BIT(_cpu._reg_P, StatusBit::Z))
        _cpu.branch();
}

void InstructionDecoder::BIT()
{
    _alu.checkZeroNegative(_cpu._reg_DBB);
    _alu.checkOverflow(_cpu._reg_DBB & 0x40); // TODO: What are this used for?
}

void InstructionDecoder::BMI()
{
    if (GET_BIT(_cpu._reg_P, StatusBit::N))
        _cpu.branch();
}

void InstructionDecoder::BNE()
{
    if (!GET_BIT(_cpu._reg_P, StatusBit::Z))
        _cpu.branch();
}

void InstructionDecoder::BPL()
{
    if (!GET_BIT(_cpu._reg_P, StatusBit::N))
        _cpu.branch();
}

void InstructionDecoder::BRK()
{
    // TODO: ???
}

void InstructionDecoder::BVC()
{
    if (!GET_BIT(_cpu._reg_P, StatusBit::V))
        _cpu.branch();
}

void InstructionDecoder::BVS()
{
    if (GET_BIT(_cpu._reg_P, StatusBit::V))
        _cpu.branch();
}

void InstructionDecoder::CLC()
{
    CLR_BIT(_cpu._reg_P, StatusBit::C);
}

void InstructionDecoder::CLD()
{
    CLR_BIT(_cpu._reg_P, StatusBit::D);
}

void InstructionDecoder::CLI()
{
    CLR_BIT(_cpu._reg_P, StatusBit::I);
}

void InstructionDecoder::CLV()
{
    CLR_BIT(_cpu._reg_P, StatusBit::V);
}

void InstructionDecoder::CMP()
{
    _alu.CMP();
}

void InstructionDecoder::CPX()
{
    _cpu._reg_A = _cpu._reg_X;
    _alu.CMP();
}

void InstructionDecoder::CPY()
{
    _cpu._reg_A = _cpu._reg_X;
    _alu.CMP();
}

void InstructionDecoder::DEC()
{
    _cpu._reg_A = _cpu._reg_DBB;
    _alu.DEC();
    _cpu._reg_DBB = _cpu._reg_A;
}

void InstructionDecoder::DEX()
{
    _cpu._reg_A = _cpu._reg_X;
    _alu.DEC();
    _cpu._reg_X = _cpu._reg_A;
}

void InstructionDecoder::DEY()
{
    _cpu._reg_A = _cpu._reg_Y;
    _alu.DEC();
    _cpu._reg_Y = _cpu._reg_A;
}

void InstructionDecoder::EOR()
{
    _alu.EOR();
}

void InstructionDecoder::INC()
{
    _cpu._reg_A = _cpu._reg_DBB;
    _alu.INC();
    _cpu._reg_DBB = _cpu._reg_A;
}

void InstructionDecoder::INX()
{
    _cpu._reg_A = _cpu._reg_X;
    _alu.INC();
    _cpu._reg_X = _cpu._reg_A;
}

void InstructionDecoder::INY()
{
    _cpu._reg_A = _cpu._reg_Y;
    _alu.INC();
    _cpu._reg_Y = _cpu._reg_A;
}

void InstructionDecoder::JMP()
{
    _cpu._reg_PC = _cpu._reg_DBB;
    _cpu._reg_PC <<= 8;
    _cpu._reg_PC |= _cpu._reg_DL;
}

void InstructionDecoder::JSR()
{
    // TODO: ???
    _cpu._reg_DBB = (_cpu._reg_PC + 1) >> 8;
    _cpu.push();
    _cpu._reg_DBB = (_cpu._reg_PC + 1);
    _cpu.push();
    JMP();
}

void InstructionDecoder::LDA()
{
    _alu.checkZeroNegative(_cpu._reg_DBB);
    _cpu._reg_A = _cpu._reg_DBB;
}

void InstructionDecoder::LDX()
{
    _alu.checkZeroNegative(_cpu._reg_DBB);
    _cpu._reg_X = _cpu._reg_DBB;
}

void InstructionDecoder::LDY()
{
    _alu.checkZeroNegative(_cpu._reg_DBB);
    _cpu._reg_Y = _cpu._reg_DBB;
}

void InstructionDecoder::LSR()
{
    _alu.LSR();
}

void InstructionDecoder::NOP()
{
    /* Just does nothing as all, as named */
}

void InstructionDecoder::ORA()
{
    _alu.ORA();
}

void InstructionDecoder::PHA()
{
    _cpu._reg_DBB = _cpu._reg_A;
    _cpu.push();
}

void InstructionDecoder::PHP()
{
    _cpu._reg_DBB = _cpu._reg_P;
    _cpu.push();
}

void InstructionDecoder::PLA()
{
    _cpu.pop();
    _cpu._reg_A = _cpu._reg_DBB;
    _alu.checkZeroNegative(_cpu._reg_A);
}

void InstructionDecoder::PLP()
{
    _cpu.pop();
    _cpu._reg_P = _cpu._reg_DBB;
}

void InstructionDecoder::ROL()
{
    _alu.ROL();
}

void InstructionDecoder::ROR()
{
    _alu.ROR();
}

void InstructionDecoder::RTI()
{
    _cpu.pop();
    _cpu._reg_P = _cpu._reg_DBB;
    _cpu.popTwo();
    JMP();
}

void InstructionDecoder::RTS()
{
    _cpu.popTwo();
    JMP();
    ++_cpu._reg_PC; // TODO: ???
}

void InstructionDecoder::SBC()
{
    _alu.SBC();
}

void InstructionDecoder::SEC()
{
    SEL_BIT(_cpu._reg_P, StatusBit::C);
}

void InstructionDecoder::SED()
{
    SEL_BIT(_cpu._reg_P, StatusBit::D);
}

void InstructionDecoder::SEI()
{
    SEL_BIT(_cpu._reg_P, StatusBit::I);
}

void InstructionDecoder::STA()
{
    _cpu._reg_DBB = _cpu._reg_A;
}

void InstructionDecoder::STX()
{
    _cpu._reg_DBB = _cpu._reg_X;
}

void InstructionDecoder::STY()
{
    _cpu._reg_DBB = _cpu._reg_Y;
}

void InstructionDecoder::TAX()
{
    _alu.checkZeroNegative(_cpu._reg_A);
    _cpu._reg_X = _cpu._reg_A;
}

void InstructionDecoder::TAY()
{
    _alu.checkZeroNegative(_cpu._reg_A);
    _cpu._reg_Y = _cpu._reg_A;
}

void InstructionDecoder::TSX()
{
    _alu.checkZeroNegative(_cpu._reg_S);
    _cpu._reg_X = _cpu._reg_S;
}

void InstructionDecoder::TXA()
{
    _alu.checkZeroNegative(_cpu._reg_X);
    _cpu._reg_A = _cpu._reg_X;
}

void InstructionDecoder::TXS()
{
    _cpu._reg_S = _cpu._reg_X;
}

void InstructionDecoder::TYA()
{
    _alu.checkZeroNegative(_cpu._reg_Y);
    _cpu._reg_A = _cpu._reg_Y;
}

inline bool InstructionDecoder::hasOperands(const Operation_t *operation)
{
    return operation->mode->operands;
}

inline bool InstructionDecoder::needsToLoad(const Operation_t *operation)
{
    return operation->type->kind & InstructionReadMask;
}

inline bool InstructionDecoder::needsToSave(const Operation_t *operation)
{
    return operation->type->kind & InstructionWriteMask;
}

} // namespace cpu

/* MicroProcessor */

MicroProcessor::MicroProcessor(Bus &bus)
    : Tickable(1)
    , _skip(0)
    , _decoder(*this)
    , _alu(*this)
    , _bus(bus)
{
}

MicroProcessor::~MicroProcessor() {}

void MicroProcessor::reset()
{
    _skip = 0;

    _reg_A = 0;
    _reg_X = 0;
    _reg_Y = 0;
    _reg_S = cpu::DefaultStack;
    _reg_P = cpu::DefaultStatus;

    // Load PC from reset vector
    _reg_PC = cpu::ResetVector;
    fetchTwo();
    reg::mergeTwoBytes(_reg_PC, _reg_DBB, _reg_DL);
}

void MicroProcessor::step()
{
    while (_skip) {
        _tick();
    }

    _tick();
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
    fetch();

    // Execute
    _decoder.load();
    _decoder.execute();
    _decoder.save();
}

inline void MicroProcessor::read()
{
    _bus.read(_reg_AB, _reg_DBB);
}

inline void MicroProcessor::write()
{
    _bus.write(_reg_AB, _reg_DBB);
}

inline void MicroProcessor::push()
{
    _reg_AB = _reg_S--;
    write();
}

inline void MicroProcessor::pop()
{
    _reg_AB = ++_reg_S;
    read();
}

inline void MicroProcessor::branch()
{
    read();
    _reg_PC += _reg_DBB;
}

inline void MicroProcessor::fetch()
{
    (this->*(_decoder._operation->mode->fetcher))();
}

void MicroProcessor::fetchNull() {
    /* No operand needed */
}

void MicroProcessor::fetchImmediate()
{
    fetchOne();
}

void MicroProcessor::fetchAbsolute()
{
    fetchTwo();
    reg::mergeTwoBytes(_reg_AB, _reg_DBB, _reg_DL);
}

void MicroProcessor::fetchZeroPage()
{
    fetchOne();
    _reg_AB = _reg_DBB;
}

void MicroProcessor::fetchIndexedZeroPageX()
{
    fetchIndexedZeroPage(_reg_X);
}

void MicroProcessor::fetchIndexedZeroPageY()
{
    fetchIndexedZeroPage(_reg_Y);
}

void MicroProcessor::fetchIndexedAbsoluteX()
{
    fetchIndexedAbsolute(_reg_X);
}

void MicroProcessor::fetchIndexedAbsoluteY()
{
    fetchIndexedAbsolute(_reg_Y);
}

void MicroProcessor::fetchRelative()
{
    fetchOne(); // branch offset
}

void MicroProcessor::fetchIndexedIndirect()
{
    fetchOne();

    // Fetch ABL
    _reg_AB = (uint8_t)(_reg_DBB + _reg_X);
    read();

    _reg_DL = _reg_DBB;

    // Fetch ABH
    ++_reg_AB;
    read();

    reg::mergeTwoBytes(_reg_AB, _reg_DBB, _reg_DL);
}

void MicroProcessor::fetchIndirectIndexed()
{
    fetchOne();

    // Fetch ABL
    _reg_AB = _reg_DBB;
    read();

    _reg_DL = _reg_DBB;

    // Fetch ABH
    ++_reg_AB;
    read();

    reg::mergeTwoBytes(_reg_AB, _reg_DBB, _reg_DL);
    _reg_AB += _reg_Y;
}

void MicroProcessor::fetchAbsoluteIndirect()
{
    fetchTwo();

    // Fetch ABL
    reg::mergeTwoBytes(_reg_AB, _reg_DBB, _reg_DL);
    read();

    _reg_DL = _reg_DBB;

    // Fetch ABH
    ++_reg_AB;
    read();

    reg::mergeTwoBytes(_reg_AB, _reg_DBB, _reg_DL);
}

inline void MicroProcessor::popTwo()
{
    pop();
    _reg_DL = _reg_DBB;
    pop();
};

inline void MicroProcessor::fetchOne()
{
    _reg_AB = _reg_PC++;
    read();
}

inline void MicroProcessor::fetchTwo()
{
    _reg_AB = _reg_PC++;
    read();

    _reg_DL = _reg_DBB;

    _reg_AB = _reg_PC++;
    read();
}

inline void MicroProcessor::fetchIndexedZeroPage(uint8_t index)
{
    fetchOne();
    _reg_AB = (uint8_t)(_reg_DBB + index);
}

inline void MicroProcessor::fetchIndexedAbsolute(uint8_t index)
{
    fetchTwo();
    reg::mergeTwoBytes(_reg_AB, _reg_DBB, _reg_DL);
    _reg_AB += index;
}

} // namespace tones
