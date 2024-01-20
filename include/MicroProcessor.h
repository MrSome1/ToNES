#ifndef _TONES_MICROPROCESSOR_H_
#define _TONES_MICROPROCESSOR_H_

#include <array>
#include <cinttypes>
#include <functional>

#include "Instruction.h"
#include "InstructionDecoder.h"
#include "Device.h"
#include "Register.h"

namespace tones {
namespace cpu {

// TODO: Where is this mentioned
const uint16_t ResetVector = 0xfffc;
const uint8_t DefaultStatus = 0x24; // 0010 0100

/**
 * @brief Processor Status Register
 * 
 * Bit Map:
 *   7  6  5  4  3  2  1  0
 *   N  V  .  B  D  I  Z  C
 */
enum class StatusBit {
    C, // Carry
    Z, // Zero
    I, // Interupt
    D, // Decimal
    B, // Break
    n, // Null
    O, // Overflow
    N, // Negative
};

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

    /* One operand operations
     *
     * These operations only work on the accumulator
     */

    //! Increment the accumulator by one
    void INC();
    
    //! Decrement the accumulator by one
    void DEC();

    //! Shift one bit left
    void ASL();

    //! Shift one bit right
    void LSR();

    //! Rotate one bit left
    void ROL();

    //! Rotate one bit right
    void ROR();

    /* Two operands operations
     *
     * These operations use the accumulator and the
     * data bus buffer register simultaneously
     */

    //! Copy to the accumultor with status
    void LDA();

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

    /* Registers refered to the cpu */
    uint8_t &_reg_A;
    uint8_t &_reg_DBB;
    // StatusRegister_t &_reg_P;
    reg::Bitwise_t &_reg_P;

    /* Not sure if there is a real register in ALU, just
     * for convtnience to check overflow and carry here
     */
    uint16_t _reg;
};

} // namespace cpu

/**
 * @brief CPU
 * 
 */
class MicroProcessor
{

public:

    /* Accessible registers of CPU */
    typedef struct Registers {
        uint8_t A;   // accumulator
        uint8_t X;   // index register X
        uint8_t Y;   // index register Y
        uint8_t S;   // program counter
        uint8_t P;   // status register
        uint16_t PC; // stack pointer
    } Registers_t;

    MicroProcessor(Bus &bus);
    ~MicroProcessor();

    void reset();

    void tick();

    void dump(Registers_t &registers) const;

protected:

    //! Read one byte from memory
    inline void read() { _bus.read(_reg_AB, _reg_DBB); }

    //! Write one byte to memory
    inline void write() { _bus.write(_reg_AB, _reg_DBB); }

    //! Push into stack
    inline void push() { _reg_AB = _reg_S--; write(); }

    //! Pop from stack
    inline void pop() { _reg_AB = ++_reg_S; read(); }

    //! Take the execution branch
    inline void branch() { read(); _reg_PC += _reg_DBB; }

    /* Functions for addressing modes */
    
    //! Dose not fetch
    static void fetchNull(MicroProcessor &cpu);

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

    //! Relative
    static void fetchRelative(MicroProcessor &cpu);

    //! (IND, X)
    static void fetchIndexedIndirect(MicroProcessor &cpu);

    //! (IND), Y
    static void fetchIndirectIndexed(MicroProcessor &cpu);

    //! Indirect
    static void fetchAbsoluteIndirect(MicroProcessor &cpu);

    /* Helper Functions */

    //! Pop from stack twice, continuously
    inline void popTwo() { pop(); _reg_DL = _reg_DBB; pop(); };

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
    uint8_t _reg_X;   // index register X
    uint8_t _reg_Y;   // index register Y
    uint8_t _reg_S;   // program counter
    uint16_t _reg_PC; // stack pointer

    /* Internal Registers */
    uint8_t _reg_IR;  // instruction register
    uint8_t _reg_DL;  // data latch
    uint8_t _reg_DBB; // data bus buffer
    uint16_t _reg_AB; // adress buffer

    reg::Bitwise_t _reg_P; // processor status register

    cpu::InstructionDecoder _decoder;
    cpu::ArithmeticAndLogicUnit _alu;

    Bus &_bus;

    static std::array<std::function<void(MicroProcessor&)>, cpu::AddressingModeCount> _fetchers;
};

} // namespace tones

#endif // _TONES_MICROPROCESSOR_H_