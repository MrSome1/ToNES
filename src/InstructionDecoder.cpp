
#include <array>

#include "InstructionDecoder.h"
#include "Instruction.h"
#include "MicroProcessor.h"

namespace tones {
namespace cpu {

const std::array<std::function<void(InstructionDecoder&)>, InstructionGroupCount>
InstructionDecoder::_groupDecoders = {
    decodeGroup0,
    decodeGroup1,
    decodeGroup2,
    decodeGroup3,
};

const std::array<std::function<void(tones::MicroProcessor&)>, InstructionGroupSize0>
InstructionDecoder::_operationGroup0 = {

};

const std::array<std::function<void(tones::MicroProcessor&)>, InstructionGroupSize1>
InstructionDecoder::_operationGroup1 = {

};

const std::array<std::function<void(tones::MicroProcessor&)>, InstructionGroupSize2>
InstructionDecoder::_operationGroup2 = {

};

const std::array<std::function<void(tones::MicroProcessor&)>, InstructionGroupSize3>
InstructionDecoder::_operationGroup3 = {

};

InstructionDecoder::InstructionDecoder(tones::MicroProcessor &cpu)
    : _cpu(cpu)
{

}

InstructionDecoder::~InstructionDecoder()
{

}

AddressingMode_t InstructionDecoder::decode()
{
    _groupDecoders[_cpu._reg_IR & InstructionGroupMask](*this);
    return _mode;
}

void InstructionDecoder::execute()
{
    _operation(_cpu);
}

/* Instruction Group 0 */

void InstructionDecoder::decodeGroup0(InstructionDecoder &decoder)
{
    // TODO
    decoder._mode = AddressingMode_t::Implied;
}

// TODO: Operations

/* Instruction Group 1 */

void InstructionDecoder::decodeGroup1(InstructionDecoder &decoder)
{
    int i;

    i = (decoder._cpu._reg_IR & InstructionNumberMask) >> InstructionNumberShift;
    decoder._operation = _operationGroup1[i];

    i = (decoder._cpu._reg_IR & AddressingModeMask) >> AddressingModeShift;
    decoder._mode = AddressingModeGroup1[i];
}

void InstructionDecoder::ORA(tones::MicroProcessor &cpu)
{
    cpu._alu.ORA();
}

void InstructionDecoder::AND(tones::MicroProcessor &cpu)
{
    cpu._alu.AND();
}

void InstructionDecoder::EOR(tones::MicroProcessor &cpu)
{
    cpu._alu.EOR();
}

void InstructionDecoder::ADC(tones::MicroProcessor &cpu)
{
    cpu._alu.EOR();

}

void InstructionDecoder::STA(tones::MicroProcessor &cpu)
{
    // TODO
}

void InstructionDecoder::LDA(tones::MicroProcessor &cpu)
{
    // TODO
}

void InstructionDecoder::CMP(tones::MicroProcessor &cpu)
{
    cpu._alu.CMP();
}

void InstructionDecoder::SBC(tones::MicroProcessor &cpu)
{
    cpu._alu.SBC();
}

/* Instruction Group 2 */

void InstructionDecoder::decodeGroup2(InstructionDecoder &decoder)
{
    // TODO
    decoder._mode = AddressingMode_t::Implied;
}

// TODO: Operations

/* Instruction Group 3 */

void InstructionDecoder::decodeGroup3(InstructionDecoder &decoder)
{
    // TODO
    decoder._mode = AddressingMode_t::Implied;
}

// TODO: Operations

} // namespace cpu
} // namespace tones