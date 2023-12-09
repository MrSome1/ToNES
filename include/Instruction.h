#ifndef _TONES_INSTRUCTION_H_
#define _TONES_INSTRUCTION_H_

#include <cinttypes>

namespace tones {
namespace cpu {

const uint8_t InstructionGroupMask = 0b00000011;
const uint8_t InstructionGroupCount = 4;

const uint8_t AddressingModeMask = 0b00011100;
const int AddressingModeShift = 2;

const uint8_t InstructionNumberMask = 0b11100000;
const int InstructionNumberShift = 5;

const int AddressingModeCount = 13;

typedef enum class AddressingMode{
    Accumulator,      // Accum
    Immediate,        // IMM
    Absolute,         // ABS
    ZeroPage,         // ZP
    IndexedZeroPageX, // ZP, X
    IndexedZeroPageY, // ZP, Y
    IndexedAbsoluteX, // ABS, X
    IndexedAbsoluteY, // ABS, Y
    Implied,          //
    Relative,         //
    IndexedIndirect,  // (IND, X)
    IndirectIndexed,  // (IND), Y
    AbsoluteIndirect  //
} AddressingMode_t;

/* Instruction Group 1 */

const int InstructionGroupSize0 = 0; // TODO

typedef enum class InstructionGroup0 {
    // TODO
} InstructionGroup0_t;

const AddressingMode_t AddressingModeGroup0[] = {
    // TODO
};

/* Instruction Group 1 */

const int InstructionGroupSize1 = 8;

typedef enum class InstructionGroup1 {
    ORA, // 'OR' memory with Accumulator
    AND, // 'AND' memory with Accumulator
    EOR, // 'Exclusive OR' with Accumulator
    ADC, // Add Memory to Accumulator with Carry
    STA, // Store Accumulator in Memory
    LDA, // Load Accumulator with Memory
    CMP, // Compare Memory and Accumulator
    SBC, // Subtract Memory from Accumulator with Borrow
} InstructionGroup1_t;

const AddressingMode_t AddressingModeGroup1[] = {
    AddressingMode::IndexedIndirect,  // (IND, X)
    AddressingMode::ZeroPage,         // ZP
    AddressingMode::Immediate,        // IMM
    AddressingMode::Absolute,         // ABS
    AddressingMode::IndirectIndexed,  // (IND), Y
    AddressingMode::IndexedZeroPageX, // ZP,X
    AddressingMode::IndexedAbsoluteY, // ABS, Y
    AddressingMode::IndexedAbsoluteX, // ABS, X
};

/* Instruction Group 1 */

const int InstructionGroupSize2 = 0; // TODO

typedef enum class InstructionGroup2 {
    // TODO
} InstructionGroup2_t;

const AddressingMode_t AddressingModeGroup2[] = {
    // TODO
};

/* Instruction Group 1 */

const int InstructionGroupSize3 = 0; // TODO

typedef enum class InstructionGroup3 {
    // TODO
} InstructionGroup3_t;

const AddressingMode_t AddressingModeGroup3[] = {
    // TODO
};

} // namespace cpu
} // namespace tones

#endif // _TONES_INSTRUCTION_H_ 