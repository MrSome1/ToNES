#ifndef _TONES_MICROPROCESSOR_H_
#define _TONES_MICROPROCESSOR_H_

#include <cinttypes>

namespace ToNES {

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

};

/**
 * @brief Decoder
 * 
 */
class InstructionDecode
{

};

/**
 * @brief CPU
 * 
 */
class MicroProcessor
{
public:
    MicroProcessor();
    ~MicroProcessor();

private:
    friend class ArithmeticAndLogicUnit;
    friend class InstructionDecode;

    /* Programmable Registers */
    uint8_t _reg_A; // accumulator
    uint8_t _reg_X; // index register Y
    uint8_t _reg_Y; // index register Y
    uint8_t _reg_S; // program counter
    uint16_t _reg_PC; // stack pointer
    StatusRegister_t _reg_P; // processor status register

    /* Buffers */
    uint8_t _reg_I; // instruction register
    uint8_t _reg_D; // data bus buffer
};

} // namespace ToNES

#endif // _TONES_MICROPROCESSOR_H_