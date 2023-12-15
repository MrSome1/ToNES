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
    Carry,    // C
    Zero,     // Z
    Interupt, // I
    Decimal,  // D
    Break,    // B
    Null,
    Overflow, // V
    Negative  // N
};

/**
 * @brief Processor Status Register
 * 
 * Bit Map:
 *   7  6  5  4  3  2  1  0
 *   N  V     B  D  I  Z  C
 */
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

    //! Copy another register to accumultor
    void load(uint8_t reg);

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

    inline void checkCarry();

    inline void checkZero();

    inline void checkOverflow();

    inline void checkNegative();

    /* Not sure if there is a real register in ALU, just
     * for convtnience to check overflow and carry here
     */
    uint16_t _reg;

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

    //! Read one byte from memory
    inline void read() { _bus.read(_reg_AB, _reg_DBB); };

    //! Write one byte to memory
    inline void write() { _bus.write(_reg_AB, _reg_DBB); };

    /* Functions for addressing modes */

    //! Accum
    static void fetchAccumulator(MicroProcessor &cpu);

    //! IMM
    static void fetchImmediate(MicroProcessor &cpu);

    //! Absolute or ABS
    static void fetchAbsolute(MicroProcessor &cpu);

    //! ZP
    static void fetchZeroPage(MicroProcessor &cpu);

    //! ZP, X
    static void fetchIndexedZeroPageX(MicroProcessor &cpu);

    //! ZP, Y
    static void fetchIndexedZeroPageY(MicroProcessor &cpu);

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

    //! Setup the register AB with two seperate bytes
    inline void setAB(uint8_t abh, uint8_t abl);

    /* Fetch one operand from memory
     * 
     * Fetch the operand of a two bytes instruction, and
     * store the operand in register DBB
     */
    static inline void fetchOne(MicroProcessor &cpu);

    /* Fetch two operands from memory
     *
     * Fetch the operands of a three bytes instruction, 
     * and store the first operand in register DL, the
     * second one in register DBB
     */ 
    static inline void fetchTwo(MicroProcessor &cpu);

    //! Fetch operands for addressing mode ZP, X or Y
    static inline void fetchIndexedZeroPage(MicroProcessor &cpu, uint8_t index);

    //! Fetch operands for addressing mode ABS, X or Y
    static inline void fetchIndexedAbsolute(MicroProcessor &cpu, uint8_t index);

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