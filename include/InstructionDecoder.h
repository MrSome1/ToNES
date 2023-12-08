#ifndef _TONES_INSTRUCTIONDECODER_H_
#define _TONES_INSTRUCTIONDECODER_H_

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

private:

    tones::MicroProcessor &_cpu;
};

} // namespace cpu
} // namespace tones

#endif // _TONES_INSTRUCTIONDECODER_H_ 