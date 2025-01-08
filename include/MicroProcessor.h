#ifndef _TONES_MICROPROCESSOR_H_
#define _TONES_MICROPROCESSOR_H_

#include <array>
#include <cinttypes>
#include <functional>

#include "Clock.h"
#include "Device.h"
#include "Register.h"
#include "Instruction.h"

namespace tones {

class MicroProcessor;

namespace cpu {

// TODO: Where is this mentioned
const uint16_t ResetVector  = 0xfffc;
const uint16_t BreakVector  = 0xfffe;
const uint8_t DefaultStack  = 0xfd;
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
    V, // Overflow
    N, // Negative
};

class InstructionDecoder;

/**
 * @brief ALU
 * 
 */
class ArithmeticAndLogicUnit
{
    friend class InstructionDecoder;
    friend class tones::MicroProcessor;

private:

    ArithmeticAndLogicUnit(tones::MicroProcessor &cpu);

    /* One operand operations
     *
     * These operations only work on the accumulator
     */

    //! Increment the accumulator by one
    inline void INC();
    
    //! Decrement the accumulator by one
    inline void DEC();

    //! Shift one bit left
    inline void ASL();

    //! Shift one bit right
    inline void LSR();

    //! Rotate one bit left
    inline void ROL();

    //! Rotate one bit right
    inline void ROR();

    /* Two operands operations
     *
     * These operations use the accumulator and the
     * data bus buffer register simultaneously
     */

    //! Operation logic 'OR'
    inline void ORA();

    //! Operation logic 'AND'
    inline void AND();

    //! Operation logic 'Exclusive OR'
    inline void EOR();

    //! Operation arithmetic 'Add'
    inline void ADC();

    //! Operation arithmetic 'Subtract'
    inline void SBC();

    //! Operation arithmetic 'Compare'
    inline void CMP();

    /* Access Status Register */

    inline void setZero(uint8_t reg);

    inline void setNegative(uint8_t reg);

    inline void setZeroNegative(uint8_t reg);

    inline void setCarry(uint8_t reg);

    inline void setOverflow(uint8_t reg);

    inline bool getZero();

    inline bool getNegative();

    inline bool getCarry();

    inline bool getOverflow();

    inline bool getDecimal();

private:

    /* Registers refered to the cpu */
    uint8_t &_reg_A;
    uint8_t &_reg_DBB;
    reg::Bitwise_t &_reg_P;
};

/**
 * @brief Instruction Decoder
 *
 */
class InstructionDecoder
{
    friend class tones::MicroProcessor;
    friend class ArithmeticAndLogicUnit;

public:

    /* Load & Store */

    void LDA();

    void LDX();

    void LDY();

    void STA();

    void STX();

    void STY();

    /* Register Transfers */

    void TAX();

    void TAY();

    void TXA();

    void TYA();

    /* Stack Instructions */

    void PHA();

    void PHP();

    void PLA();

    void PLP();

    void TSX();

    void TXS();

    /* Status Instructions */

    void CLC();

    void CLD();

    void CLI();

    void CLV();

    void SEC();

    void SED();

    void SEI();

    /* Shift Instructions */

    void ASL();

    void LSR();

    void ROL();

    void ROR();

    /* Increments & Decrements */

    void DEC();

    void DEX();

    void DEY();

    void INC();

    void INX();

    void INY();

    /* Logical Instructions */

    void AND();

    void EOR();

    void ORA();

    void BIT();

    /* Arithmetic Instructions */

    void ADC();

    void SBC();

    void CMP();

    void CPX();

    void CPY();

    /* Jumps & Calls */

    void JMP();

    void JSR();

    void RTS();

    /* Branch Instructions */

    void BCC();

    void BCS();

    void BEQ();

    void BMI();

    void BNE();

    void BPL();

    void BVC();

    void BVS();

    /* System Functions */

    void BRK();

    void NOP();

    void RTI();

private:

    InstructionDecoder(tones::MicroProcessor &cpu);

    //! Parse the operation code
    inline void decode();

    //! Run the operation
    inline void execute();

    // TODO: A better way?
    //! Execute operations of accumulate addressing mode
    inline void accumulate(void (ArithmeticAndLogicUnit::*executor)());

private:

    MicroProcessor &_cpu;

    ArithmeticAndLogicUnit &_alu;

    const Operation_t *_operation;
};

} // namespace cpu

/**
 * @brief CPU
 * 
 */
class MicroProcessor : public Tickable
{
    friend class cpu::InstructionDecoder;
    friend class cpu::ArithmeticAndLogicUnit;

public:

    typedef void (*Fetcher)(MicroProcessor&);

    /* Accessible registers of CPU */
    typedef struct Registers {
        uint8_t A;   // accumulator
        uint8_t X;   // index register X
        uint8_t Y;   // index register Y
        uint8_t S;   // stack pointer
        uint8_t P;   // status register
        uint16_t PC; // program counter
    } Registers_t;

public:

    MicroProcessor(Bus &bus);
    ~MicroProcessor();

    void reset();

    void step();

    void dump(Registers_t &registers) const;

    /* Functions for addressing modes */
    
    //! Dose not fetch
    void fetchNull();

    //! IMM
    void fetchImmediate();

    //! Absolute or ABS
    void fetchAbsolute();

    //! ZP
    void fetchZeroPage();

    //! ZP, X
    void fetchIndexedZeroPageX();

    //! ZP, Y
    void fetchIndexedZeroPageY();

    //! ABS, X
    void fetchIndexedAbsoluteX();

    //! ABS, Y
    void fetchIndexedAbsoluteY();

    //! Relative
    void fetchRelative();

    //! (IND, X)
    void fetchIndexedIndirect();

    //! (IND), Y
    void fetchIndirectIndexed();

    //! Indirect
    void fetchAbsoluteIndirect();

protected:

    void _tick() override;

    //! Read one byte from memory
    inline void read();

    //! Write one byte to memory
    inline void write();

    //! Push into stack
    inline void push();

    //! Pop from stack
    inline void pop();

    //! Take the execution branch
    inline void branch();

    //! Fetch operands
    inline void fetch();

    /* Helper Functions */

    //! Pop from stack twice, continuously
    inline void popTwo();

    /* Fetch one operand from memory
     * 
     * Fetch the operand of a two bytes instruction, and
     * store the operand in register DBB
     */
    inline void fetchOne();

    /* Fetch two operands from memory
     *
     * Fetch the operands of a three bytes instruction, 
     * and store the first operand in register DL, the
     * second one in register DBB
     */ 
    inline void fetchTwo();

    //! Fetch operands for addressing mode ZP, X or Y
    inline void fetchIndexedZeroPage(uint8_t index);

    //! Fetch operands for addressing mode ABS, X or Y
    inline void fetchIndexedAbsolute(uint8_t index);

private:

    uint8_t _skip;

    /* Programmable Registers */
    uint8_t  _reg_A;  // accumulator
    uint8_t  _reg_X;  // index register X
    uint8_t  _reg_Y;  // index register Y
    uint8_t  _reg_S;  // stack pointer
    uint16_t _reg_PC; // program counter

    reg::Bitwise_t _reg_P; // processor status register

    /* Internal Registers */
    uint8_t  _reg_IR;  // instruction register
    uint8_t  _reg_DL;  // data latch
    uint8_t  _reg_DBB; // data bus buffer
    uint16_t _reg_AB;  // adress buffer

    cpu::InstructionDecoder _decoder;
    cpu::ArithmeticAndLogicUnit _alu;

    Bus &_bus;
};

} // namespace tones

#endif // _TONES_MICROPROCESSOR_H_
