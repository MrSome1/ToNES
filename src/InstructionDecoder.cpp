
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
    _instruction = code::InstructionSet[_cpu._reg_IR];
    if (code::Unknown == _instruction->name) {
        _operation = nullptr; // TODO: safe ?
    } else {
        _operation = _operations[_instruction->name];
    }
    return _instruction->mode;
}

void InstructionDecoder::execute()
{
    _operation(_cpu);
}

void InstructionDecoder::load()
{
    if (hasOperands(_instruction) && needsToLoad(_instruction))
        _cpu.read();
}

void InstructionDecoder::save()
{
    if (hasOperands(_instruction) && needsToSave(_instruction))
        _cpu.write();
}

/* Routine Instructions */

void InstructionDecoder::BRK(tones::MicroProcessor &cpu)
{
    // TODO: ???
}

void InstructionDecoder::JSR(tones::MicroProcessor &cpu)
{
    // TODO: ???
    cpu._reg_DBB = (cpu._reg_PC + 1) >> 8;
    cpu.push();
    cpu._reg_DBB = (cpu._reg_PC + 1);
    cpu.push();
    JMP(cpu);
}

void InstructionDecoder::RTI(tones::MicroProcessor &cpu)
{
    cpu.pop();
    cpu._reg_P = cpu._reg_DBB;
    cpu.popTwo();
    JMP(cpu);
}

void InstructionDecoder::RTS(tones::MicroProcessor &cpu)
{
    cpu.popTwo();
    JMP(cpu);
    ++cpu._reg_PC; // TODO: ???
}

void InstructionDecoder::JMP(tones::MicroProcessor &cpu)
{
    cpu._reg_PC = cpu._reg_DBB;
    cpu._reg_PC <<= 8;
    cpu._reg_PC |= cpu._reg_DL;
}

/* Branch Instructions */

void InstructionDecoder::BPL(tones::MicroProcessor &cpu)
{
    if (!GET_BIT(cpu._reg_P, StatusBit::N))
        cpu.branch();
}

void InstructionDecoder::BMI(tones::MicroProcessor &cpu)
{
    if (GET_BIT(cpu._reg_P, StatusBit::N))
        cpu.branch();
}

void InstructionDecoder::BVC(tones::MicroProcessor &cpu)
{
    if (!GET_BIT(cpu._reg_P, StatusBit::O))
        cpu.branch();
}

void InstructionDecoder::BVS(tones::MicroProcessor &cpu)
{
    if (GET_BIT(cpu._reg_P, StatusBit::O))
        cpu.branch();
}

void InstructionDecoder::BCC(tones::MicroProcessor &cpu)
{
    if (!GET_BIT(cpu._reg_P, StatusBit::C))
        cpu.branch();
}

void InstructionDecoder::BCS(tones::MicroProcessor &cpu)
{
    if (GET_BIT(cpu._reg_P, StatusBit::C))
        cpu.branch();
}

void InstructionDecoder::BNE(tones::MicroProcessor &cpu)
{
    if (!GET_BIT(cpu._reg_P, StatusBit::Z))
        cpu.branch();
}

void InstructionDecoder::BEQ(tones::MicroProcessor &cpu)
{
    if (GET_BIT(cpu._reg_P, StatusBit::Z))
        cpu.branch();
}

/* Stack Instructions */

void InstructionDecoder::PHP(tones::MicroProcessor &cpu)
{
    cpu._reg_DBB = cpu._reg_P;
    cpu.push();
}

void InstructionDecoder::PLP(tones::MicroProcessor &cpu)
{
    cpu.pop();
    cpu._reg_P = cpu._reg_DBB;
}

void InstructionDecoder::PHA(tones::MicroProcessor &cpu)
{
    cpu._reg_DBB = cpu._reg_A;
    cpu.push();
}

void InstructionDecoder::PLA(tones::MicroProcessor &cpu)
{
    cpu.pop();
    cpu._reg_A = cpu._reg_DBB;
}

/* Status Instructions */

void InstructionDecoder::CLC(tones::MicroProcessor &cpu)
{
    SET_BIT(cpu._reg_P, StatusBit::C, false);
}

void InstructionDecoder::SEC(tones::MicroProcessor &cpu)
{
    SET_BIT(cpu._reg_P, StatusBit::C, true);
}

void InstructionDecoder::CLI(tones::MicroProcessor &cpu)
{
    SET_BIT(cpu._reg_P, StatusBit::I, false);
}

void InstructionDecoder::SEI(tones::MicroProcessor &cpu)
{
    SET_BIT(cpu._reg_P, StatusBit::I, true);
}

void InstructionDecoder::CLV(tones::MicroProcessor &cpu)
{
    SET_BIT(cpu._reg_P, StatusBit::O, false);
}

void InstructionDecoder::CLD(tones::MicroProcessor &cpu)
{
    SET_BIT(cpu._reg_P, StatusBit::D, false);
}

void InstructionDecoder::SED(tones::MicroProcessor &cpu)
{
    SET_BIT(cpu._reg_P, StatusBit::D, true);
}

/* Index Instructions */

void InstructionDecoder::DEY(tones::MicroProcessor &cpu)
{
    cpu._reg_A = cpu._reg_Y;
    cpu._alu.DEC();
    cpu._reg_Y = cpu._reg_A;
}

void InstructionDecoder::INY(tones::MicroProcessor &cpu)
{
    cpu._reg_A = cpu._reg_Y;
    cpu._alu.INC();
    cpu._reg_Y = cpu._reg_A;
}

void InstructionDecoder::INX(tones::MicroProcessor &cpu)
{
    cpu._reg_A = cpu._reg_X;
    cpu._alu.INC();
    cpu._reg_X = cpu._reg_A;
}

void InstructionDecoder::DEX(tones::MicroProcessor &cpu)
{
    cpu._reg_A = cpu._reg_X;
    cpu._alu.DEC();
    cpu._reg_X = cpu._reg_A;
}

/* Transfer Instructions */

void InstructionDecoder::TYA(tones::MicroProcessor &cpu)
{
    cpu._reg_DBB = cpu._reg_Y;
    cpu._alu.LDA();
}

void InstructionDecoder::TAY(tones::MicroProcessor &cpu)
{
    cpu._reg_Y = cpu._reg_A;
}

void InstructionDecoder::TXA(tones::MicroProcessor &cpu)
{
    cpu._reg_DBB = cpu._reg_X;
    cpu._alu.LDA();
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
    // TODO: ???
    cpu._alu.LDA();
}

void InstructionDecoder::STY(tones::MicroProcessor &cpu)
{
    cpu._reg_DBB = cpu._reg_Y;
}

void InstructionDecoder::LDY(tones::MicroProcessor &cpu)
{
    cpu._reg_Y = cpu._reg_DBB;
}

void InstructionDecoder::CPY(tones::MicroProcessor &cpu)
{
    cpu._reg_A = cpu._reg_X;
    cpu._alu.CMP();
}

void InstructionDecoder::CPX(tones::MicroProcessor &cpu)
{
    cpu._reg_A = cpu._reg_X;
    cpu._alu.CMP();
}

/* Instruction Group 1 */

void InstructionDecoder::ORA(tones::MicroProcessor &cpu)
{
    cpu._alu.ORA();
}

void InstructionDecoder::AND(tones::MicroProcessor &cpu)
{
    cpu._alu.AND();
}

void InstructionDecoder::EOR(tones::MicroProcessor &cpu)
{
    cpu._alu.EOR();
}

void InstructionDecoder::ADC(tones::MicroProcessor &cpu)
{
    cpu._alu.ADC();
}

void InstructionDecoder::STA(tones::MicroProcessor &cpu)
{
    cpu._reg_DBB = cpu._reg_A;
}

void InstructionDecoder::LDA(tones::MicroProcessor &cpu)
{
    cpu._reg_A = cpu._reg_DBB;
}

void InstructionDecoder::CMP(tones::MicroProcessor &cpu)
{
    cpu._alu.CMP();
}

void InstructionDecoder::SBC(tones::MicroProcessor &cpu)
{
    cpu._alu.SBC();
}

/* Instruction Group 2 */

void InstructionDecoder::ASL(tones::MicroProcessor &cpu)
{
    cpu._alu.ASL();
}

void InstructionDecoder::ROL(tones::MicroProcessor &cpu)
{
    cpu._alu.ROL();
}

void InstructionDecoder::LSR(tones::MicroProcessor &cpu)
{
    cpu._alu.LSR();
}

void InstructionDecoder::ROR(tones::MicroProcessor &cpu)
{
    cpu._alu.ROR();
}

void InstructionDecoder::STX(tones::MicroProcessor &cpu)
{
    cpu._reg_DBB = cpu._reg_X;
}

void InstructionDecoder::LDX(tones::MicroProcessor &cpu)
{
    cpu._alu.LDA();
    cpu._reg_X = cpu._reg_A;
}

void InstructionDecoder::DEC(tones::MicroProcessor &cpu)
{
    cpu._reg_A = cpu._reg_DBB;
    cpu._alu.DEC();
    cpu._reg_DBB = cpu._reg_A;
}

void InstructionDecoder::INC(tones::MicroProcessor &cpu)
{
    cpu._reg_A = cpu._reg_DBB;
    cpu._alu.INC();
    cpu._reg_DBB = cpu._reg_A;
}

/* Helper Functions */

inline bool InstructionDecoder::hasOperands(const code::Instruction_t *instruction)
{
    return instruction->mode & 0xfe; // mode > Accum
}

inline bool InstructionDecoder::needsToLoad(const code::Instruction_t *instruction)
{
    return code::ReadWriteModeSet[instruction->name] & InstructionReadMask;
}

inline bool InstructionDecoder::needsToSave(const code::Instruction_t *instruction)
{
    return code::ReadWriteModeSet[instruction->name] & InstructionWriteMask;
}

} // namespace cpu
} // namespace tones