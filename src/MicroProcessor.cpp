
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
    setZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::DEC()
{
    --_reg_A;
    setZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::ASL()
{
    setCarry(_reg_A & 0x80);
    _reg_A <<= 1;
    setZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::LSR()
{
    setCarry(_reg_A & 0x01);
    _reg_A >>= 1;
    setZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::ROL()
{
    _reg_A <<= 1;
    _reg_A |= getCarry() ? 0x01 : 0x00;
    setCarry(_reg_DBB & 0x80);
    setZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::ROR()
{
    _reg_A >>= 1;
    _reg_A |= getCarry() ? 0x80 : 0x00;
    setCarry(_reg_DBB & 0x01);
    setZeroNegative(_reg_A);
}

/* Two operands operations */

void ArithmeticAndLogicUnit::ORA()
{
    _reg_A |= _reg_DBB;
    setZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::AND()
{
    _reg_A &= _reg_DBB;
    setZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::EOR()
{
    _reg_A ^= _reg_DBB;
    setZeroNegative(_reg_A);
}

void ArithmeticAndLogicUnit::ADC()
{
    // TODO: Without tmp?
    uint16_t tmp = _reg_A + _reg_DBB + (getCarry() ? 1 : 0);
    // Signed overflow is true when the result sign differs from both operands
    setOverflow((_reg_A ^ tmp) & (_reg_DBB ^ tmp) & 0x80);
    setCarry(tmp & 0x100); // unsigned overflow
    _reg_A = tmp & 0xff;
    setZeroNegative(_reg_A);
    // No desimal mode in NES
}

void ArithmeticAndLogicUnit::SBC()
{
    // TODO: What does it mean ?
    // TODO: Without tmp?
    uint16_t tmp = _reg_A - _reg_DBB - (getCarry() ? 0 : 1);
    setCarry(!(tmp & 0x100));
    setOverflow((_reg_A ^ tmp) & (~_reg_DBB ^ tmp) & 0x80);
    _reg_A = tmp & 0xff;
    setZeroNegative(_reg_A);
    // No desimal mode in NES
}

void ArithmeticAndLogicUnit::CMP()
{
    // TODO: What does it mean ?
    std::uint16_t tmp = _reg_A - _reg_DBB;
    setCarry(!(tmp & 0x100));
    setZeroNegative(tmp);
    // _reg_A -= _reg_DBB;
    // setCarry(_reg_A & 0x80); // TODO: ???
    // setZeroNegative(_reg_A);
}

inline void ArithmeticAndLogicUnit::setZero(uint8_t reg)
{
    SET_BIT(_reg_P, StatusBit::Z, !reg);
}

inline void ArithmeticAndLogicUnit::setNegative(uint8_t reg)
{
    SET_BIT(_reg_P, StatusBit::N, reg & 0x80);
}

inline void ArithmeticAndLogicUnit::setZeroNegative(uint8_t reg)
{
    setZero(reg);
    setNegative(reg);
}

inline void ArithmeticAndLogicUnit::setCarry(bool val)
{
    SET_BIT(_reg_P, StatusBit::C, val);
}

inline void ArithmeticAndLogicUnit::setOverflow(bool val)
{
    SET_BIT(_reg_P, StatusBit::V, val);
}

inline bool ArithmeticAndLogicUnit::getZero()
{
    return GET_BIT(_reg_P, StatusBit::Z);
}

inline bool ArithmeticAndLogicUnit::getNegative()
{
    return GET_BIT(_reg_P, StatusBit::N);
}

inline bool ArithmeticAndLogicUnit::getCarry()
{
    return GET_BIT(_reg_P, StatusBit::C);
}

inline bool ArithmeticAndLogicUnit::getOverflow()
{
    return GET_BIT(_reg_P, StatusBit::V);
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

inline void InstructionDecoder::accumulate(void (ArithmeticAndLogicUnit::*executor)())
{
    if (cpu::code::Accumulator == _operation->mode->kind) {
        _cpu._reg_DBB = _cpu._reg_A;
        (_alu.*executor)();
    } else {
        _cpu._reg_DL = _cpu._reg_A;
        _cpu._reg_A = _cpu._reg_DBB;
        (_alu.*executor)();
        _cpu._reg_DBB = _cpu._reg_A;
        _cpu._reg_A = _cpu._reg_DL;
        _cpu.write();
    }
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
    accumulate(&ArithmeticAndLogicUnit::ASL);
}

void InstructionDecoder::BCC()
{
    if (!_alu.getCarry()) {
        // TODO: Dynamic clock
        _cpu.branch();
    }
}

void InstructionDecoder::BCS()
{
    if (_alu.getCarry()) {
        // TODO: Dynamic clock
        _cpu.branch();
    }
}

void InstructionDecoder::BEQ()
{
    if (_alu.getZero()) {
        // TODO: Dynamic clock
        _cpu.branch();
    }
}

void InstructionDecoder::BIT()
{
    _alu.setNegative(_cpu._reg_DBB);
    _alu.setOverflow(_cpu._reg_DBB & 0x40);
    _alu.setZero(_cpu._reg_DBB & _cpu._reg_A);
}

void InstructionDecoder::BMI()
{
    if (_alu.getNegative()) {
        // TODO: Dynamic clock
        _cpu.branch();
    }
}

void InstructionDecoder::BNE()
{
    if (!_alu.getZero()) {
        // TODO: Dynamic clock
        _cpu.branch();
    }
}

void InstructionDecoder::BPL()
{
    if (!_alu.getNegative()) {
        // TODO: Dynamic clock
        _cpu.branch();
    }
}

void InstructionDecoder::BRK()
{
    ++_cpu._reg_PC; // 6502 quirk
    SEL_BIT(_cpu._reg_P, cpu::StatusBit::B);
    _cpu.interrupt(cpu::VectorIRQ);
    CLR_BIT(_cpu._reg_P, cpu::StatusBit::B);
}

void InstructionDecoder::BVC()
{
    if (!_alu.getOverflow()) {
        // TODO: Dynamic clock
        _cpu.branch();
    }
}

void InstructionDecoder::BVS()
{
    if (_alu.getOverflow()) {
        // TODO: Dynamic clock
        _cpu.branch();
    }
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
    _cpu._reg_DL = _cpu._reg_A;
    _alu.CMP();
    _cpu._reg_A = _cpu._reg_DL;
}

void InstructionDecoder::CPX()
{
    _cpu._reg_DL = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_X;
    _alu.CMP();
    _cpu._reg_A = _cpu._reg_DL;
}

void InstructionDecoder::CPY()
{
    _cpu._reg_DL = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_Y;
    _alu.CMP();
    _cpu._reg_A = _cpu._reg_DL;
}

void InstructionDecoder::DEC()
{
    _cpu._reg_DL = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_DBB;
    _alu.DEC();
    _cpu._reg_DBB = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_DL;
    _cpu.write();
}

void InstructionDecoder::DEX()
{
    _cpu._reg_DL = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_X;
    _alu.DEC();
    _cpu._reg_X = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_DL;
}

void InstructionDecoder::DEY()
{
    _cpu._reg_DL = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_Y;
    _alu.DEC();
    _cpu._reg_Y = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_DL;
}

void InstructionDecoder::EOR()
{
    _alu.EOR();
}

void InstructionDecoder::INC()
{
    _cpu._reg_DL = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_DBB;
    _alu.INC();
    _cpu._reg_DBB = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_DL;
    _cpu.write();
}

void InstructionDecoder::INX()
{
    _cpu._reg_DL = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_X;
    _alu.INC();
    _cpu._reg_X = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_DL;
}

void InstructionDecoder::INY()
{
    _cpu._reg_DL = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_Y;
    _alu.INC();
    _cpu._reg_Y = _cpu._reg_A;
    _cpu._reg_A = _cpu._reg_DL;
}

void InstructionDecoder::JMP()
{
    _cpu._reg_PC = _cpu._reg_AB;
}

void InstructionDecoder::JSR()
{
    --_cpu._reg_PC;

    reg::splitTwoBytes(_cpu._reg_PC, _cpu._reg_DBB, _cpu._reg_DL);
    _cpu._reg_PC = _cpu._reg_AB;

    _cpu.push();
    _cpu._reg_DBB = _cpu._reg_DL;
    _cpu.push();
}

void InstructionDecoder::LDA()
{
    _alu.setZeroNegative(_cpu._reg_DBB);
    _cpu._reg_A = _cpu._reg_DBB;
}

void InstructionDecoder::LDX()
{
    _alu.setZeroNegative(_cpu._reg_DBB);
    _cpu._reg_X = _cpu._reg_DBB;
}

void InstructionDecoder::LDY()
{
    _alu.setZeroNegative(_cpu._reg_DBB);
    _cpu._reg_Y = _cpu._reg_DBB;
}

void InstructionDecoder::LSR()
{
    accumulate(&ArithmeticAndLogicUnit::LSR);
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
    // The break bit is always set, which is not defined
    // in 6502.txt, but is consistent with FCEUX
    _cpu._reg_DBB = _cpu._reg_P;
    SEL_BIT(_cpu._reg_DBB, cpu::StatusBit::B);
    _cpu.push();
}

void InstructionDecoder::PLA()
{
    _cpu.pop();
    _cpu._reg_A = _cpu._reg_DBB;
    _alu.setZeroNegative(_cpu._reg_A);
}

void InstructionDecoder::PLP()
{
    _cpu.pop();
    SET_BIT(_cpu._reg_DBB, cpu::StatusBit::B,
            GET_BIT(_cpu._reg_P, cpu::StatusBit::B));
    _cpu._reg_P = _cpu._reg_DBB | cpu::ClearedStatus;
}

void InstructionDecoder::ROL()
{
    accumulate(&ArithmeticAndLogicUnit::ROL);
}

void InstructionDecoder::ROR()
{
    accumulate(&ArithmeticAndLogicUnit::ROR);
}

void InstructionDecoder::RTI()
{
    _cpu.pop();
    _cpu._reg_P = _cpu._reg_DBB | cpu::ClearedStatus;
    _cpu.popTwo();
    reg::mergeTwoBytes(_cpu._reg_PC, _cpu._reg_DBB, _cpu._reg_DL);
}

void InstructionDecoder::RTS()
{
    _cpu.popTwo();
    reg::mergeTwoBytes(_cpu._reg_PC, _cpu._reg_DBB, _cpu._reg_DL);
    ++_cpu._reg_PC;
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
    _cpu.write();
}

void InstructionDecoder::STX()
{
    _cpu._reg_DBB = _cpu._reg_X;
    _cpu.write();
}

void InstructionDecoder::STY()
{
    _cpu._reg_DBB = _cpu._reg_Y;
    _cpu.write();
}

void InstructionDecoder::TAX()
{
    _alu.setZeroNegative(_cpu._reg_A);
    _cpu._reg_X = _cpu._reg_A;
}

void InstructionDecoder::TAY()
{
    _alu.setZeroNegative(_cpu._reg_A);
    _cpu._reg_Y = _cpu._reg_A;
}

void InstructionDecoder::TSX()
{
    _alu.setZeroNegative(_cpu._reg_S);
    _cpu._reg_X = _cpu._reg_S;
}

void InstructionDecoder::TXA()
{
    _alu.setZeroNegative(_cpu._reg_X);
    _cpu._reg_A = _cpu._reg_X;
}

void InstructionDecoder::TXS()
{
    _cpu._reg_S = _cpu._reg_X;
}

void InstructionDecoder::TYA()
{
    _alu.setZeroNegative(_cpu._reg_Y);
    _cpu._reg_A = _cpu._reg_Y;
}

} // namespace cpu

/* MicroProcessor */

MicroProcessor::MicroProcessor(Bus &bus)
    : _skip(0)
    , _decoder(*this)
    , _alu(*this)
    , _bus(bus)
{
}

MicroProcessor::~MicroProcessor() {}

void MicroProcessor::tick()
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

    _decoder.execute();
}

void MicroProcessor::step()
{
    while (_skip) {
        tick();
    }

    tick();
}

void MicroProcessor::reset()
{
    _skip = 0;

    _reg_A = 0;
    _reg_X = 0;
    _reg_Y = 0;
    _reg_S = cpu::DefaultStack;
    _reg_P = cpu::DefaultStatus;

    // Load PC from reset vector
    _reg_PC = cpu::VectorRST;
    fetchTwo();
    reg::mergeTwoBytes(_reg_PC, _reg_DBB, _reg_DL);
}

void MicroProcessor::MicroProcessor::irq()
{
    if (!GET_BIT(_reg_P, cpu::StatusBit::I)) {
        interrupt(cpu::VectorIRQ);
        // TODO: Cycle time
    }
}

void MicroProcessor::MicroProcessor::nmi()
{
    interrupt(cpu::VectorNMI);
    // TODO: Cycle time
}

void MicroProcessor::wait(uint8_t ticks)
{
    _skip += ticks;
}

void MicroProcessor::jump(uint16_t addr)
{
    _reg_PC = addr;
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

void MicroProcessor::interrupt(uint16_t vector)
{
    // Save register PC to the stack
    reg::getMSB(_reg_PC, _reg_DBB);
    push();
    reg::getLSB(_reg_PC, _reg_DBB);
    push();

    // Save register P to the stack
    _reg_DBB = _reg_P;
    push();

    SEL_BIT(_reg_P, cpu::StatusBit::I);

    // Load PC from break vector
    _reg_PC = vector;
    fetchTwo();
    reg::mergeTwoBytes(_reg_PC, _reg_DBB, _reg_DL);

    _skip += 7;
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
    _reg_AB = cpu::StackBase | _reg_S--;
    write();
}

inline void MicroProcessor::pop()
{
    _reg_AB = cpu::StackBase | ++_reg_S;
    read();
}

inline void MicroProcessor::branch()
{
    _reg_PC += (int8_t)_reg_DBB;
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
    read();
}

void MicroProcessor::fetchZeroPage()
{
    fetchOne();
    _reg_AB = _reg_DBB;
    read();
}

void MicroProcessor::fetchIndexedZeroPageX()
{
    fetchIndexedZeroPage(_reg_X);
    read();
}

void MicroProcessor::fetchIndexedZeroPageY()
{
    fetchIndexedZeroPage(_reg_Y);
    read();
}

void MicroProcessor::fetchIndexedAbsoluteX()
{
    fetchIndexedAbsolute(_reg_X);
    read();
}

void MicroProcessor::fetchIndexedAbsoluteY()
{
    fetchIndexedAbsolute(_reg_Y);
    read();
}

void MicroProcessor::fetchRelative()
{
    fetchOne(); // branch offset
}

void MicroProcessor::fetchIndexedIndirect()
{
    fetchOne();

    // Fetch ABL
    _reg_AB = _reg_DBB + _reg_X;
    _reg_AB &= 0xff;
    read();

    _reg_DL = _reg_DBB;

    // Fetch ABH
    ++_reg_AB;
    _reg_AB &= 0xff;
    read();

    reg::mergeTwoBytes(_reg_AB, _reg_DBB, _reg_DL);
    read();
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
    _reg_AB &= 0xff;
    read();

    reg::mergeTwoBytes(_reg_AB, _reg_DBB, _reg_DL);
    _reg_AB += _reg_Y;
    read();
}

void MicroProcessor::fetchAbsoluteIndirect()
{
    fetchTwo();

    // Fetch ABL
    reg::mergeTwoBytes(_reg_AB, _reg_DBB, _reg_DL);
    read();

    _reg_DL = _reg_DBB;

    // Fetch ABH
    // 6502 CPU has a bug, that when an indirect address begins at
    // the last byte of one page, the second byte is fetched from
    // the first byte of the page, rather than the next byte.
    // So, just '++_reg_AB' will not work here.
    _reg_AB = (_reg_AB & 0xff00) | ((_reg_AB + 1) & 0xff);
    read();

    reg::mergeTwoBytes(_reg_AB, _reg_DBB, _reg_DL);
    read();
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
