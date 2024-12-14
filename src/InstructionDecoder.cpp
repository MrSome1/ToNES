
#include <array>

#include "InstructionDecoder.h"
#include "Instruction.h"
#include "MicroProcessor.h"
#include "Register.h"

namespace tones {
namespace cpu {

InstructionDecoder::InstructionDecoder(tones::MicroProcessor &cpu)
    : _cpu(cpu)
    , _alu(cpu._alu)
    , _operation(&UnknownOperation)
{

}

code::AddressingKind_t InstructionDecoder::mode() const
{
    return _operation->mode->kind;
}

void InstructionDecoder::decode()
{
    _operation = getOperation(_cpu._reg_IR);
    _cpu._skip = _operation->cycles; // TODO: Dynamic cycles
}

void InstructionDecoder::execute()
{
    (this->*(_operation->type->execution))();
}

void InstructionDecoder::load()
{
    if (hasOperands(_operation) && needsToLoad(_operation))
        _cpu.read();
}

void InstructionDecoder::save()
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
    //_alu.checkOverflow(_cpu._reg_DBB & 0x40); // TODO: What are this used for?
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

} // namespace cpu
} // namespace tones
