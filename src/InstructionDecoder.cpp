
#include <array>

#include "InstructionDecoder.h"
#include "Instruction.h"
#include "MicroProcessor.h"

namespace tones {
namespace cpu {

const std::array<std::function<void(tones::MicroProcessor&)>, code::Unknown>
InstructionDecoder::_operations = {
    ADC, AND, ASL,
    BCC, BCS, BEQ, BIT, BMI, BNE, BPL, BRK, BVC, BVS,
    CLC, CLD, CLI, CLV, CMP, CPX, CPY,
    DEC, DEX, DEY,
    EOR,
    INC, INX, INY,
    JMP, JSR,
    LDA, LDX, LDY, LSR,
    NOP,
    ORA,
    PHA, PHP, PLA, PLP,
    ROL, ROR, RTI, RTS,
    SBC, SEC, SED, SEI, STA, STX, STY,
    TAX, TAY, TSX, TXA, TXS, TYA,
};

InstructionDecoder::InstructionDecoder(tones::MicroProcessor &cpu)
    : _cpu(cpu)
{

}

InstructionDecoder::~InstructionDecoder()
{

}

code::AddressingMode_t InstructionDecoder::decode()
{
    auto *instruction = code::InstructionSet[_cpu._reg_IR];
    if (code::Unknown == instruction ->name) {
        _operation = nullptr;
    } else {
        _operation = _operations[instruction->name];
    }
    return instruction->mode;
}

void InstructionDecoder::execute()
{
    _operation(_cpu);
}

/* Routine Instructions */

void InstructionDecoder::BRK(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::JSR(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::RTI(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::RTS(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::JMP(tones::MicroProcessor &cpu)
{

}

/* Branch Instructions */

void InstructionDecoder::BPL(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::BMI(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::BVC(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::BVS(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::BCC(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::BCS(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::BNE(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::BEQ(tones::MicroProcessor &cpu)
{

}

/* Stack Instructions */

void InstructionDecoder::PHP(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::PLP(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::PHA(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::PLA(tones::MicroProcessor &cpu)
{

}

/* Status Instructions */

void InstructionDecoder::CLC(tones::MicroProcessor &cpu)
{
    cpu._reg_P.set(StatusBit::Carry, false);
}

void InstructionDecoder::SEC(tones::MicroProcessor &cpu)
{
    cpu._reg_P.set(StatusBit::Carry, true);
}

void InstructionDecoder::CLI(tones::MicroProcessor &cpu)
{
    cpu._reg_P.set(StatusBit::Interupt, false);
}

void InstructionDecoder::SEI(tones::MicroProcessor &cpu)
{
    cpu._reg_P.set(StatusBit::Interupt, true);
}

void InstructionDecoder::CLV(tones::MicroProcessor &cpu)
{
    cpu._reg_P.set(StatusBit::Overflow, false);
}

void InstructionDecoder::CLD(tones::MicroProcessor &cpu)
{
    cpu._reg_P.set(StatusBit::Decimal, false);
}

void InstructionDecoder::SED(tones::MicroProcessor &cpu)
{
    cpu._reg_P.set(StatusBit::Decimal, true);
}

/* Index Instructions */

void InstructionDecoder::DEY(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::INY(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::INX(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::DEX(tones::MicroProcessor &cpu)
{

}

/* Transfer Instructions */

void InstructionDecoder::TYA(tones::MicroProcessor &cpu)
{
    cpu._alu.load(cpu._reg_Y);
}

void InstructionDecoder::TAY(tones::MicroProcessor &cpu)
{
    cpu._reg_Y = cpu._reg_A;
}

void InstructionDecoder::TXA(tones::MicroProcessor &cpu)
{
    cpu._alu.load(cpu._reg_X);
}

void InstructionDecoder::TXS(tones::MicroProcessor &cpu)
{
    cpu._reg_S = cpu._reg_X;
}

void InstructionDecoder::TAX(tones::MicroProcessor &cpu)
{
    cpu._reg_X = cpu._reg_A;
}

void InstructionDecoder::TSX(tones::MicroProcessor &cpu)
{
    cpu._reg_X = cpu._reg_S; // check zero and negative ???
}

/* No Operation */

void InstructionDecoder::NOP(tones::MicroProcessor &cpu)
{
    /* Just does nothing as all, as named */
}

/* Instruction Group 0 */

void InstructionDecoder::BIT(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::STY(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::LDY(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::CPY(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::CPX(tones::MicroProcessor &cpu)
{

}

/* Instruction Group 1 */

void InstructionDecoder::ORA(tones::MicroProcessor &cpu)
{
    cpu.read();
    cpu._alu.ORA();
}

void InstructionDecoder::AND(tones::MicroProcessor &cpu)
{
    cpu.read();
    cpu._alu.AND();
}

void InstructionDecoder::EOR(tones::MicroProcessor &cpu)
{
    cpu.read();
    cpu._alu.EOR();
}

void InstructionDecoder::ADC(tones::MicroProcessor &cpu)
{
    cpu.read();
    cpu._alu.ADC();
}

void InstructionDecoder::STA(tones::MicroProcessor &cpu)
{
    cpu._reg_DBB = cpu._reg_A;
    cpu.write();
}

void InstructionDecoder::LDA(tones::MicroProcessor &cpu)
{
    cpu.read();
    cpu._reg_A = cpu._reg_DBB;
}

void InstructionDecoder::CMP(tones::MicroProcessor &cpu)
{
    cpu.read();
    cpu._alu.CMP();
}

void InstructionDecoder::SBC(tones::MicroProcessor &cpu)
{
    cpu.read();
    cpu._alu.SBC();
}

/* Instruction Group 2 */

void InstructionDecoder::ASL(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::ROL(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::LSR(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::ROR(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::STX(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::LDX(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::DEC(tones::MicroProcessor &cpu)
{

}

void InstructionDecoder::INC(tones::MicroProcessor &cpu)
{

}

} // namespace cpu
} // namespace tones