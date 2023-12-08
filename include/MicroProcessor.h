#ifndef _TONES_MICROPROCESSOR_H_
#define _TONES_MICROPROCESSOR_H_

#include <cinttypes>

#include "Instruction.h"
#include "InstructionDecoder.h"
#include "DataBus.h"

namespace tones {
namespace cpu {

enum class StatusBit {
    Carry,
    Zero,
    Interupt,
    Decimal,
    Break,
    Overflow = 6,
    Negative
};

typedef struct StatusRegister {
    uint8_t status;

    StatusRegister();

    void reset();

    void set(StatusBit bit, bool value);

    bool get(StatusBit bit);

} StatusRegister_t;

class MicroProcessor;

/**
 * @brief ALU
 * 
 */
class ArithmeticAndLogicUnit
{

public:
    ArithmeticAndLogicUnit(tones::MicroProcessor &cpu);
    ~ArithmeticAndLogicUnit();

private:
    tones::MicroProcessor &_cpu;
};

} // namespace cpu

/**
 * @brief CPU
 * 
 */
class MicroProcessor
{

public:
    MicroProcessor(DataBus &bus);
    ~MicroProcessor();

    void step();

private:
    friend class cpu::InstructionDecoder;
    friend class cpu::ArithmeticAndLogicUnit;

    /* Programmable Registers */
    uint8_t _reg_A; // accumulator
    uint8_t _reg_X; // index register Y
    uint8_t _reg_Y; // index register Y
    uint8_t _reg_S; // program counter
    uint16_t _reg_PC; // stack pointer
    cpu::StatusRegister_t _reg_P; // processor status register

    /* Buffers */
    uint8_t _reg_I; // instruction register
    uint8_t _reg_D; // data bus buffer

    cpu::InstructionDecoder _decoder;
    cpu::ArithmeticAndLogicUnit _alu;

    DataBus &_bus;
};

} // namespace tones

#endif // _TONES_MICROPROCESSOR_H_