#ifndef _TONES_INSTRUCTIONDECODER_H_
#define _TONES_INSTRUCTIONDECODER_H_

#include <array>
#include <functional>

#include "Instruction.h"

namespace tones {

class MicroProcessor;

namespace cpu {

/**
 * @brief Instruction Decoder
 * 
 */
class InstructionDecoder
{

public:
    InstructionDecoder(tones::MicroProcessor &cpu);
    ~InstructionDecoder();

    code::AddressingMode_t decode();

    void execute();

protected:

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

    /* Un-grouped Instructions */

    static void BRK(tones::MicroProcessor &cpu);

    static void BPL(tones::MicroProcessor &cpu);

    static void JSR(tones::MicroProcessor &cpu);

    static void BMI(tones::MicroProcessor &cpu);

    static void RTI(tones::MicroProcessor &cpu);

    static void BVC(tones::MicroProcessor &cpu);

    static void RTS(tones::MicroProcessor &cpu);

    static void BVS(tones::MicroProcessor &cpu);

    static void BCC(tones::MicroProcessor &cpu);

    static void BCS(tones::MicroProcessor &cpu);

    static void BNE(tones::MicroProcessor &cpu);

    static void BEQ(tones::MicroProcessor &cpu);

    static void PHP(tones::MicroProcessor &cpu);

    static void CLC(tones::MicroProcessor &cpu);

    static void PLP(tones::MicroProcessor &cpu);

    static void SEC(tones::MicroProcessor &cpu);

    static void PHA(tones::MicroProcessor &cpu);

    static void CLI(tones::MicroProcessor &cpu);

    static void PLA(tones::MicroProcessor &cpu);

    static void SEI(tones::MicroProcessor &cpu);

    static void DEY(tones::MicroProcessor &cpu);

    static void TYA(tones::MicroProcessor &cpu);

    static void TAY(tones::MicroProcessor &cpu);

    static void CLV(tones::MicroProcessor &cpu);

    static void INY(tones::MicroProcessor &cpu);

    static void CLD(tones::MicroProcessor &cpu);

    static void INX(tones::MicroProcessor &cpu);

    static void SED(tones::MicroProcessor &cpu);

    static void TXA(tones::MicroProcessor &cpu);

    static void TXS(tones::MicroProcessor &cpu);

    static void TAX(tones::MicroProcessor &cpu);

    static void TSX(tones::MicroProcessor &cpu);

    static void DEX(tones::MicroProcessor &cpu);

    static void NOP(tones::MicroProcessor &cpu);

    static void JMP(tones::MicroProcessor &cpu);

private:

    tones::MicroProcessor &_cpu;

    code::AddressingMode_t _mode;

    std::function<void(tones::MicroProcessor&)> _operation;

    static const std::array<std::function<void(tones::MicroProcessor&)>,
                            code::Unknown> _operations;
};

} // namespace cpu
} // namespace tones

#endif // _TONES_INSTRUCTIONDECODER_H_ 