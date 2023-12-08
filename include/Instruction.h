#ifndef _TONES_INSTRUCTION_H_
#define _TONES_INSTRUCTION_H_

#include <cinttypes>

namespace tones {
namespace cpu {

const uint8_t InstructionGroupMask = 0b00000011;

const uint8_t AddressingModeMask = 0b00011100;
const int AddressingModeShift = 2;

const uint8_t InstructionNumberMask = 0b11100000;
const int InstructionNumberShift = 5;

typedef enum AddressingMode{
    Accumulator,
    Immediate,
    Absolute,
    ZeroPage,
    IndexedZeroPageX,
    IndexedZeroPageY,
    IndexedAbsolueX,
    IndexedAbsolueY,
    Implied,
    Relative,
    IndexedIndirect,
    IndirectIndexed,
    AbsoluteIndirect
} AddressingMode_t;

} // namespace cpu
} // namespace tones

#endif // _TONES_INSTRUCTION_H_ 