#ifndef _TONES_INSTRUCTION_H_
#define _TONES_INSTRUCTION_H_

#include <array>
#include <cinttypes>

namespace tones {
namespace cpu {

/** The Operation Code
 *
 * According to the op code matrix, 6502 processors
 * use one byte as the operation code, as bellow:
 * 
 *   Bit  7  6  5  4  3  2  1  0
 *        -------  -------  ----
 *           |        |      |
 *           |        |      +-- Operation Group
 *           |        +--------- Addressing Mode
 *           +------------------ Instruction Number
 * 
 * Operations in the same group do have something
 * in common, and share similar addressing modes.
 * 
 * Group 0 is especially for implied instructions,
 * like routines, branch, stack and transfer.
 * 
 * Group 1 and 2 mainly hold arithmetic and logic
 * instructions.
 *
 * Group 3 is totally empty by now, maybe used for
 * some extention instructions
 */

const int InstructionGroupMask  = 0x3;   // 0000 0011
const int InstructionGroupCount = 4;

const int AddressingModeMask  = 0x1c;    // 0001 1100
const int AddressingModeShift = 2;

const int InstructionNumberMask  = 0xe0; // 1110 0000
const int InstructionNumberShift = 5;

const int AddressingModeCount = 13;
const int InstructionSetSize  = 256;

const int InstructionReadMask  = 0x01;   // 0000 0001
const int InstructionWriteMask = 0x02;   // 0000 0010

namespace code {

/* Addressing Modes */
typedef enum AddressingKind {
    Implied,          //
    Accumulator,      // Accum
    Immediate,        // IMM
    Absolute,         // ABS
    ZeroPage,         // ZP
    IndexedZeroPageX, // ZP, X
    IndexedZeroPageY, // ZP, Y
    IndexedAbsoluteX, // ABS, X
    IndexedAbsoluteY, // ABS, Y
    Relative,         //
    IndexedIndirect,  // (IND, X)
    IndirectIndexed,  // (IND), Y
    AbsoluteIndirect, // Indirect  

    Invalid
} AddressingKind_t;

/* Read and Write Mode
 *
 * This means if an instruction needs to read from or
 * write to the main memory. This is specific for each
 * instruction actually, so this mode was not defined
 * explicitly in the 6502 manual, defined it here just
 * for decoding the instructions conveniently
 */
typedef enum ReadWriteMode {
    NO,
    R,
    W,
    RW
} ReadWriteMode_t;

/* Instructions */
typedef enum InstructionKind {
    ADC, // Add Memory to Accumulator with Carry
    AND, // 'AND' Memory with Accumulator
    ASL, // Shift Left One Bit (Memory or Accumulator)

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
    LSR, // Shift One Bit Right (Memory or Accumulator)

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
} InstructionKind_t;

} // namespace code

/* Addressing Mode */
typedef struct AddressingMode {
    const code::AddressingKind_t kind;
    const int operands;
    const char *name;
} AddressingMode_t;

/* Instruction */
typedef struct Instruction {
    const code::InstructionKind_t kind;
    const code::ReadWriteMode_t mode;
    const char *name;
} Instruction_t;

/* Operation */
typedef struct Operation {
    const Instruction_t *inst;
    const AddressingMode_t *mode;
    const int cycles;
} Operation_t;

//! The Unknown Instruction
extern const Operation_t UnknownOperation;

//! Instruction Set Op Code Matrix
extern const std::array<const Operation_t*, InstructionSetSize> OperationSet;

} // namespace cpu
} // namespace tones

#endif // _TONES_INSTRUCTION_H_
