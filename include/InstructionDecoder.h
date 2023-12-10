#ifndef _TONES_INSTRUCTIONDECODER_H_
#define _TONES_INSTRUCTIONDECODER_H_

#include <array>
#include <functional>

#include "Instruction.h"

namespace tones {

class MicroProcessor;

namespace cpu {

/**
 * @brief Instruction Decoder
 * 
 */
class InstructionDecoder
{

public:
    InstructionDecoder(tones::MicroProcessor &cpu);
    ~InstructionDecoder();

    code::AddressingMode_t decode();

    void execute();

protected:

    /* Instruction Group 0 */

    //static void decodeGroup0(InstructionDecoder &decoder);

    // TODO: Operations

    /* Instruction Group 1 */

    //static void decodeGroup1(InstructionDecoder &decoder);

    static void ORA(tones::MicroProcessor &cpu);

    static void AND(tones::MicroProcessor &cpu);

    static void EOR(tones::MicroProcessor &cpu);

    static void ADC(tones::MicroProcessor &cpu);

    static void STA(tones::MicroProcessor &cpu);

    static void LDA(tones::MicroProcessor &cpu);

    static void CMP(tones::MicroProcessor &cpu);

    static void SBC(tones::MicroProcessor &cpu);

    /* Instruction Group 2 */

    //static void decodeGroup2(InstructionDecoder &decoder);

    // TODO: Operations

    /* Instruction Group 3 */

    //static void decodeGroup3(InstructionDecoder &decoder);

    // TODO: Operations

private:

    tones::MicroProcessor &_cpu;

    code::AddressingMode_t _mode;

    std::function<void(tones::MicroProcessor&)> _operation;

    //static const std::array<std::function<void(InstructionDecoder&)>,
    //                  InstructionGroupCount> _groupDecoders;

    //static const std::array<std::function<void(tones::MicroProcessor&)>,
    //                  InstructionGroupSize0> _operationGroup0;

    //static const std::array<std::function<void(tones::MicroProcessor&)>,
    //                  InstructionGroupSize1> _operationGroup1;

    //static const std::array<std::function<void(tones::MicroProcessor&)>,
    //                  InstructionGroupSize2> _operationGroup2;

    //static const std::array<std::function<void(tones::MicroProcessor&)>,
    //                  InstructionGroupSize3> _operationGroup3;
};

} // namespace cpu
} // namespace tones

#endif // _TONES_INSTRUCTIONDECODER_H_ 