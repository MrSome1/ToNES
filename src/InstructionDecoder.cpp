
#include "InstructionDecoder.h"
#include "Instruction.h"
#include "MicroProcessor.h"

namespace tones {
namespace cpu {

InstructionDecoder::InstructionDecoder(tones::MicroProcessor &cpu)
    : _cpu(cpu)
{

}

InstructionDecoder::~InstructionDecoder()
{

}

} // namespace cpu
} // namespace tones