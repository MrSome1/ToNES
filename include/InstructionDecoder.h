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

protected:

    /* Routine Instructions */

    inline void BRK();

    inline void JSR();

    inline void RTI();

    inline void RTS();

    inline void JMP();

    /* Branch Instructions */

    inline void BPL();

    inline void BMI();

    inline void BVC();

    inline void BVS();

    inline void BCC();

    inline void BCS();

    inline void BNE();

    inline void BEQ();

    /* Stack Instructions */

    inline void PHP();

    inline void PLP();

    inline void PHA();

    inline void PLA();

    /* Status Instructions */

    inline void CLC();

    inline void SEC();

    inline void CLI();

    inline void SEI();

    inline void CLV();

    inline void CLD();

    inline void SED();

    /* Index Instructions */

    inline void DEY();

    inline void INY();

    inline void INX();

    inline void DEX();

    /* Transfer Instructions */

    inline void TYA();

    inline void TAY();

    inline void TXA();

    inline void TXS();

    inline void TAX();

    inline void TSX();

    /* No Operation */

    inline void NOP();

    /* Instruction Group 0 */

    inline void BIT();

    inline void STY();

    inline void LDY();

    inline void CPY();

    inline void CPX();

    /* Instruction Group 1 */

    inline void ORA();

    inline void AND();

    inline void EOR();

    inline void ADC();

    inline void STA();

    inline void LDA();

    inline void CMP();

    inline void SBC();

    /* Instruction Group 2 */

    inline void ASL();

    inline void ROL();

    inline void LSR();

    inline void ROR();

    inline void STX();

    inline void LDX();

    inline void DEC();

    inline void INC();

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
