#ifndef _TONES_INSTRUCTION_H_
#define _TONES_INSTRUCTION_H_

namespace tones {

class MicroProcessor;

namespace cpu {

class InstructionDecoder;

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

const int OperationGroupMask  = 0x3;   // 0000 0011
const int OperationGroupCount = 4;

const int AddressingModeMask  = 0x1c;    // 0001 1100
const int AddressingModeShift = 2;

const int InstructionNumberMask  = 0xe0; // 1110 0000
const int InstructionNumberShift = 5;

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
} AddressingKind_t;

/* Instructions */
typedef enum InstructionKind {
    ADC, // R   Add Memory to Accumulator with Carry
    AND, // R   'AND' Memory with Accumulator
    ASL, // RW  Shift Left One Bit (Memory or Accumulator)

    BCC, // R   Branch on Carry Clear
    BCS, // R   Branch on Carry Set
    BEQ, // R   Branch on Result Zero
    BIT, // R   Test Bits in Memory with Accumulator
    BMI, // R   Branch on Result Minus
    BNE, // R   Branch on Result not Zero
    BPL, // R   Branch on Result Plus
    BRK, // -   Force Break
    BVC, // R   Branch on Overflow Clear
    BVS, // R   Branch on Overflow Set

    CLC, // -   Clear Carry Flag
    CLD, // -   Clear Decimal Mode
    CLI, // -   Clear Interrupt Disable Bit
    CLV, // -   Clear Overflow Flag
    CMP, // R   Compare Memory and Accumulator
    CPX, // R   Compare Memory and Index X
    CPY, // R   Compare Memory and Index Y

    DEC, // RW  Decrement Memory by One
    DEX, // -   Decrement Index X by One
    DEY, // -   Decrement Index Y by One

    EOR, // R   'Exclusive OR' with Accumulator

    INC, // RW  Increment Memory by One
    INX, // -   Increment Index X by One
    INY, // -   Increment Index Y by One

    JMP, // R   Jump to New Location
    JSR, // R   Jump to New Location Saving Return Address

    LDA, // R   Load Accumulator with Memory
    LDX, // R   Load Index X with Memory
    LDY, // R   Load Index Y with Memory
    LSR, // RW  Shift One Bit Right (Memory or Accumulator)

    NOP, // -   No Operation

    ORA, // R   'OR' Memory with Accumulator

    PHA, // -   Push Accumulator on Stack
    PHP, // -   Push Processor Status on Stack
    PLA, // -   Pull Accumulator from Stack
    PLP, // -   Pull Processor Status on Stack

    ROL, // RW  Rotate one Bit Left (Memory or Accumulator)
    ROR, // RW  Rotate one Bit Right (Memory or Accumulator)
    RTI, // -   Return from Interrupt
    RTS, // -   Return from Subroutine

    SBC, // R   Subtract Memory from Accumulator with Borrow
    SEC, // -   Set Carry Flag
    SED, // -   Set Decimal Mode
    SEI, // -   Set Interrupt Disable Status
    STA, // W   Store Accumulator in Memory
    STX, // W   Store Index X in Memory
    STY, // W   Store Index Y in Memory

    TAX, // -   Transfer Accumulator to Index X
    TAY, // -   Transfer Accumulator to Index Y
    TSX, // -   Transfer Stack Pointer to Index X
    TXA, // -   Transfer Index X to Accumulator
    TXS, // -   Transfer Index X to Stack Register
    TYA, // -   Transfer Index Y to Accumulator
} InstructionKind_t;

} // namespace code

/* Addressing Mode */
typedef struct AddressingMode {
    void (MicroProcessor::*fetcher)();
    const code::AddressingKind_t kind;
    const int operands;
    const char* const name;
} AddressingMode_t;

/* Instruction */
typedef struct Instruction {
    void (InstructionDecoder::*executor)();
    const code::InstructionKind_t kind;
    const char* const name;
} Instruction_t;

/* Operation */
typedef struct Operation {
    const Instruction_t* const type;
    const AddressingMode_t* const mode;
    const int cycles;
} Operation_t;

//! Count of Addressing Modes
const int AddressingModeSize = code::AbsoluteIndirect + 1;

//! Count of Valid Instructions
const int InstructionSetSize = code::TYA + 1;

//! Count of All Operation Codes
const int OperationSetSize = 256;

//! The Unknown Instruction
extern const Operation_t UnknownOperation;

//! Instruction Set Op Code Matrix
extern const Operation_t* const OperationSet[OperationSetSize];

} // namespace cpu
} // namespace tones

#endif // _TONES_INSTRUCTION_H_
