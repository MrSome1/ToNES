
#include "PictureProcessingUnit.h"

namespace tones {

/* Registers */

PictureProcessingUnit::Registers::Registers(PictureProcessingUnit &ppu) : _ppu(ppu) {}

bool PictureProcessingUnit::Registers::contains(uint16_t addr) const
{
    return addr >= ppu::PPUCTRL && addr <= ppu::PPUDATA;
}

void PictureProcessingUnit::Registers::read(uint16_t address, uint8_t &buffer) const
{
    /* TODO */
    buffer = _ppu._reg_DL;
}

void PictureProcessingUnit::Registers::write(uint16_t address, uint8_t data)
{
    _ppu._reg_DL = data;
    /* TODO */
}

/* PictureProcessingUnit */

PictureProcessingUnit::PictureProcessingUnit(Bus &vbus)
    : _vbus(vbus)
    , _registers(*this)
{

}

void PictureProcessingUnit::attach(Bus &bus)
{
    _registers.attach(bus);
}

void PictureProcessingUnit::tick()
{
    /* TODO */
}

void PictureProcessingUnit::readPPUSTATUS(uint8_t &byte)
{

}

void PictureProcessingUnit::readOAMDATA(uint8_t &byte)
{

}

void PictureProcessingUnit::readPPUDATA(uint8_t &byte)
{

}

void PictureProcessingUnit::writePPUCTRL(uint8_t byte)
{

}

void PictureProcessingUnit::writePPUMASK(uint8_t byte)
{

}

void PictureProcessingUnit::writeOAMADDR(uint8_t byte)
{

}

void PictureProcessingUnit::writeOAMDATA(uint8_t byte)
{

}

void PictureProcessingUnit::writePPUSCROLL(uint8_t byte)
{

}

void PictureProcessingUnit::writePPUADDR(uint8_t byte)
{

}

void PictureProcessingUnit::writePPUDATA(uint8_t byte)
{

}

} // namespace tones