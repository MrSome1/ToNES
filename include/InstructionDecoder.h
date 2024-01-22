#ifndef _TONES_INSTRUCTIONDECODER_H_
#define _TONES_INSTRUCTIONDECODER_H_

#include <array>

#include "Instruction.h"

namespace tones {

class MicroProcessor;

namespace cpu {

typedef void (*Operation_t)(tones::MicroProcessor&);

/**
 * @brief Instruction Decoder
 * 
 */
class InstructionDecoder
{

public:
    InstructionDecoder(tones::MicroProcessor &cpu);
    ~InstructionDecoder();

    //! Current adrressing mode
    code::AddressingMode_t mode() const;

    //! Parse the operation code
    void decode();

    //! Run the operation
    void execute();

    //! Load the content of memory
    void load();

    //! Store to memory
    void save();

protected:

    /* Routine Instructions */

    static void BRK(tones::MicroProcessor &cpu);

    static void JSR(tones::MicroProcessor &cpu);

    static void RTI(tones::MicroProcessor &cpu);

    static void RTS(tones::MicroProcessor &cpu);

    static void JMP(tones::MicroProcessor &cpu);

    /* Branch Instructions */

    static void BPL(tones::MicroProcessor &cpu);

    static void BMI(tones::MicroProcessor &cpu);

    static void BVC(tones::MicroProcessor &cpu);

    static void BVS(tones::MicroProcessor &cpu);

    static void BCC(tones::MicroProcessor &cpu);

    static void BCS(tones::MicroProcessor &cpu);

    static void BNE(tones::MicroProcessor &cpu);

    static void BEQ(tones::MicroProcessor &cpu);

    /* Stack Instructions */

    static void PHP(tones::MicroProcessor &cpu);

    static void PLP(tones::MicroProcessor &cpu);

    static void PHA(tones::MicroProcessor &cpu);

    static void PLA(tones::MicroProcessor &cpu);

    /* Status Instructions */

    static void CLC(tones::MicroProcessor &cpu);

    static void SEC(tones::MicroProcessor &cpu);

    static void CLI(tones::MicroProcessor &cpu);

    static void SEI(tones::MicroProcessor &cpu);

    static void CLV(tones::MicroProcessor &cpu);

    static void CLD(tones::MicroProcessor &cpu);

    static void SED(tones::MicroProcessor &cpu);

    /* Index Instructions */

    static void DEY(tones::MicroProcessor &cpu);

    static void INY(tones::MicroProcessor &cpu);

    static void INX(tones::MicroProcessor &cpu);

    static void DEX(tones::MicroProcessor &cpu);

    /* Transfer Instructions */

    static void TYA(tones::MicroProcessor &cpu);

    static void TAY(tones::MicroProcessor &cpu);

    static void TXA(tones::MicroProcessor &cpu);

    static void TXS(tones::MicroProcessor &cpu);

    static void TAX(tones::MicroProcessor &cpu);

    static void TSX(tones::MicroProcessor &cpu);

    /* No Operation */

    static void NOP(tones::MicroProcessor &cpu);

    /* Instruction Group 0 */

    static void BIT(tones::MicroProcessor &cpu);

    static void STY(tones::MicroProcessor &cpu);

    static void LDY(tones::MicroProcessor &cpu);

    static void CPY(tones::MicroProcessor &cpu);

    static void CPX(tones::MicroProcessor &cpu);

    /* Instruction Group 1 */

    static void ORA(tones::MicroProcessor &cpu);

    static void AND(tones::MicroProcessor &cpu);

    static void EOR(tones::MicroProcessor &cpu);

    static void ADC(tones::MicroProcessor &cpu);

    static void STA(tones::MicroProcessor &cpu);

    static void LDA(tones::MicroProcessor &cpu);

    static void CMP(tones::MicroProcessor &cpu);

    static void SBC(tones::MicroProcessor &cpu);

    /* Instruction Group 2 */

    static void ASL(tones::MicroProcessor &cpu);

    static void ROL(tones::MicroProcessor &cpu);

    static void LSR(tones::MicroProcessor &cpu);

    static void ROR(tones::MicroProcessor &cpu);

    static void STX(tones::MicroProcessor &cpu);

    static void LDX(tones::MicroProcessor &cpu);

    static void DEC(tones::MicroProcessor &cpu);

    static void INC(tones::MicroProcessor &cpu);

    /* Helper Functions */

    static bool hasOperands(const code::Instruction_t *instruction);

    static bool needsToLoad(const code::Instruction_t *instruction);

    static bool needsToSave(const code::Instruction_t *instruction);

private:

    tones::MicroProcessor &_cpu;

    const code::Instruction_t *_instruction;

    static const std::array<Operation_t, code::Unknown + 1> _operations;
};

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

#endif // _TONES_INSTRUCTIONDECODER_H_ 