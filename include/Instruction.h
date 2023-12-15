#ifndef _TONES_INSTRUCTION_H_
#define _TONES_INSTRUCTION_H_

#include <array>
#include <cinttypes>

namespace tones {
namespace cpu {

/* The Instruction Code
 *
 * According to the op code matrix, 6502 processors
 * use one byte as the operation code, as bellow:
 * 
 *   Bit  7  6  5  4  3  2  1  0
 *        -------  -------  ----
 *           |        |      |
 *           |        |      +-- Instruction Group
 *           |        +--------- Addressing Mode
 *           +------------------ Instruction Number
 * 
 * Instructions in the same group do have something
 * in common, and share similar addressing modes.
 * 
 * Group 0 is especially for implied instructions,
 * like routines, branch, stack and transfer.
 * 
 * Group 1 and 2 mainly hold arithmetic and logic
 * instructions.
 *
 * Group 3 is totally empty by now, maybe used for
 * some extention operations
 */

const uint8_t InstructionGroupMask = 0b00000011;
const uint8_t InstructionGroupCount = 4;

const uint8_t AddressingModeMask = 0b00011100;
const int AddressingModeShift = 2;

const uint8_t InstructionNumberMask = 0b11100000;
const int InstructionNumberShift = 5;

const int AddressingModeCount = 13;
const int InstructionSetSize = 256;

namespace code {

/* Addressing Mode */
typedef enum AddressingMode {
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
    AbsoluteIndirect, // Indirect  

    Invalid
} AddressingMode_t;

/* Alphabetic List of Instruction Set */
typedef enum InstructionName {
    ADC, // Add Memory to Accumulator with Carry
    AND, // 'AND' Memory with Accumulator
    ASL, // Shift one Bit Left (Memory or Accumulator)

    BCC, // Branch on Carry Clear
    BCS, // Branch on Carry Set
    BEQ, // Branch on Result Zero
    BIT, // Test Bits in Memory with Accumulator
    BMI, // Branch on Result Minus
    BNE, // Branch on Result not Zero
    BPL, // Branch on Result Plus
    BRK, // Force Break
    BVC, // Branch on Overflow Clear
    BVS, // Branch on Overflow Set

    CLC, // Clear Carry Flag
    CLD, // Clear Decimal Mode
    CLI, // Clear Interrupt Disable Bit
    CLV, // Clear Overflow Flag
    CMP, // Compare Memory and Accumulator
    CPX, // Compare Memory and Index X
    CPY, // Compare Memory and Index Y

    DEC, // Decrement Memory by One
    DEX, // Decrement Index X by One
    DEY, // Decrement Index Y by One

    EOR, // 'Exclusive OR' with Accumulator

    INC, // Increment Memory by One
    INX, // Increment Index X by One
    INY, // Increment Index Y by One

    JMP, // Jump to New Location
    JSR, // Jump to New Location Saving Return Address

    LDA, // Load Accumulator with Memory
    LDX, // Load Index X with Memory
    LDY, // Load Index Y with Memory
    LSR, // Shift one Bit Right (Memory or Accumulator)

    NOP, // No Operation

    ORA, // 'OR' Memory with Accumulator

    PHA, // Push Accumulator on Stack
    PHP, // Push Processor Status on Stack
    PLA, // Pull Accumulator from Stack
    PLP, // Pull Processor Status on Stack

    ROL, // Rotate one Bit Left (Memory or Accumulator)
    ROR, // Rotate one Bit Right (Memory or Accumulator)
    RTI, // Return from Interrupt
    RTS, // Return from Subroutine

    SBC, // Subtract Memory from Accumulator with Borrow
    SEC, // Set Carry Flag
    SED, // Set Decimal Mode
    SEI, // Set Interrupt Disable Status
    STA, // Store Accumulator in Memory
    STX, // Store Index X in Memory
    STY, // Store Index Y in Memory

    TAX, // Transfer Accumulator to Index X
    TAY, // Transfer Accumulator to Index Y
    TSX, // Transfer Stack Pointer to Index X
    TXA, // Transfer Index X to Accumulator
    TXS, // Transfer Index X to Stack Register
    TYA, // Transfer Index Y to Accumulator

    Unknown
} InstructionName_t;

typedef struct Instruction {
    InstructionName_t name;
    AddressingMode mode;
    int cycles;
} Instruction_t;

const Instruction_t UnknownInstruction = {
    Unknown,
    Invalid,
    0
};

extern const std::array<const Instruction_t*, InstructionSetSize> InstructionSet;

} // namespace code
} // namespace cpu
} // namespace tones

#endif // _TONES_INSTRUCTION_H_ 