#ifndef _TONES_MICROPROCESSOR_H_
#define _TONES_MICROPROCESSOR_H_

#include <array>
#include <cinttypes>
#include <functional>

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

    //! Operation logic 'OR'
    void ORA();

    //! Operation logic 'AND'
    void AND();

    //! Operation logic 'Exclusive OR'
    void EOR();

    //! Operation arithmetic 'Add'
    void ADC();

    //! Operation arithmetic 'Subtract'
    void SBC();

    //! Operation arithmetic 'Compare'
    void CMP();

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

protected:

    inline void read();

    inline void write();

    /* Functions to fetch operands */

    //! Accum
    static void fetchAccumulator(MicroProcessor &cpu);

    //! IMM
    static void fetchImmediate(MicroProcessor &cpu);

    //! Absolute or ABS
    static void fetchAbsolute(MicroProcessor &cpu);

    //! ZP
    static void fetchZeroPage(MicroProcessor &cpu);

    //! ZP, X or Y
    static void fetchIndexedZeroPage(MicroProcessor &cpu, uint8_t index);

    //! ZP, X
    static void fetchIndexedZeroPageX(MicroProcessor &cpu);

    //! ZP, Y
    static void fetchIndexedZeroPageY(MicroProcessor &cpu);

    //! ABS, X or Y
    static void fetchIndexedAbsolute(MicroProcessor &cpu, uint8_t index);

    //! ABS, X
    static void fetchIndexedAbsoluteX(MicroProcessor &cpu);

    //! ABS, Y
    static void fetchIndexedAbsoluteY(MicroProcessor &cpu);
    
    //! Implied
    static void fetchImplied(MicroProcessor &cpu);

    //! Relative
    static void fetchRelative(MicroProcessor &cpu);

    //! (IND, X)
    static void fetchIndexedIndirect(MicroProcessor &cpu);

    //! (IND), Y
    static void fetchIndirectIndexed(MicroProcessor &cpu);

    //! Indirect
    static void fetchAbsoluteIndirect(MicroProcessor &cpu);

    /* Helper Functions */

    [[deprecated]] inline void setABL(uint8_t val);

    [[deprecated]] inline void setABH(uint8_t val);

    inline void setAB(uint8_t abh, uint8_t abl);

private:

    friend class cpu::InstructionDecoder;
    friend class cpu::ArithmeticAndLogicUnit;

    /* Programmable Registers */
    uint8_t _reg_A;   // accumulator
    uint8_t _reg_X;   // index register Y
    uint8_t _reg_Y;   // index register Y
    uint8_t _reg_S;   // program counter
    uint16_t _reg_PC; // stack pointer

    /* Internal Registers */
    uint8_t _reg_IR;  // instruction register
    uint8_t _reg_DL;  // data latch
    uint8_t _reg_DBB; // data bus buffer
    uint16_t _reg_AB; // adress buffer

    cpu::StatusRegister_t _reg_P; // processor status register

    cpu::InstructionDecoder _decoder;
    cpu::ArithmeticAndLogicUnit _alu;

    DataBus &_bus;

    static std::array<std::function<void(MicroProcessor&)>, cpu::AddressingModeCount> _fetchers;
};

} // namespace tones

#endif // _TONES_MICROPROCESSOR_H_