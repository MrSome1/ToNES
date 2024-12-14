#ifndef _TONES_INSTRUCTIONDECODER_H_
#define _TONES_INSTRUCTIONDECODER_H_

#include <array>
#include <cinttypes>

#include "Instruction.h"

namespace tones {

class MicroProcessor;

namespace cpu {

class ArithmeticAndLogicUnit;

/**
 * @brief Instruction Decoder
 * 
 */
class InstructionDecoder
{

public:

    InstructionDecoder(tones::MicroProcessor &cpu);
    ~InstructionDecoder() = default;

    //! Current addressing mode
    code::AddressingKind_t mode() const;

    //! Parse the operation code
    void decode();

    //! Run the operation
    void execute();

    //! Load the content of memory
    void load();

    //! Store to memory
    void save();

    //! Get the instruction according to the op code
    static const Operation_t *getOperation(uint16_t op);

public:

    /* Routine Instructions */

    void BRK();

    void JSR();

    void RTI();

    void RTS();

    void JMP();

    /* Branch Instructions */

    void BPL();

    void BMI();

    void BVC();

    void BVS();

    void BCC();

    void BCS();

    void BNE();

    void BEQ();

    /* Stack Instructions */

    void PHP();

    void PLP();

    void PHA();

    void PLA();

    /* Status Instructions */

    void CLC();

    void SEC();

    void CLI();

    void SEI();

    void CLV();

    void CLD();

    void SED();

    /* Index Instructions */

    void DEY();

    void INY();

    void INX();

    void DEX();

    /* Transfer Instructions */

    void TYA();

    void TAY();

    void TXA();

    void TXS();

    void TAX();

    void TSX();

    /* No Operation */

    void NOP();

    /* Instruction Group 0 */

    void BIT();

    void STY();

    void LDY();

    void CPY();

    void CPX();

    /* Instruction Group 1 */

    void ORA();

    void AND();

    void EOR();

    void ADC();

    void STA();

    void LDA();

    void CMP();

    void SBC();

    /* Instruction Group 2 */

    void ASL();

    void ROL();

    void LSR();

    void ROR();

    void STX();

    void LDX();

    void DEC();

    void INC();

    /* Helper Functions */

    static bool hasOperands(const Operation_t *operation);

    static bool needsToLoad(const Operation_t *operation);

    static bool needsToSave(const Operation_t *operation);

private:

    MicroProcessor &_cpu;

    ArithmeticAndLogicUnit &_alu;

    const Operation_t *_operation;
};

inline const Operation_t *InstructionDecoder::getOperation(uint16_t op)
{
    return OperationSet[op];
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
} // namespace tones

#endif // _TONES_INSTRUCTIONDECODER_H_
