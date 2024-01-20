
#include "PictureProcessingUnit.h"
#include "Register.h"

namespace tones {
namespace ppu {

} // namespace ppu

/* Registers */

PictureProcessingUnit::Registers::Registers(PictureProcessingUnit &ppu) : _ppu(ppu) {}

bool PictureProcessingUnit::Registers::contains(uint16_t addr) const
{
    return addr >= ppu::PPUCTRL && addr <= ppu::PPUDATA;
}

void PictureProcessingUnit::Registers::read(uint16_t address, uint8_t &buffer) const
{
    switch (address) {
        case ppu::PPUSTATUS: _ppu.readPPUSTATUS(); break;
        case ppu::OAMDATA:   _ppu.readOAM();       break;
        case ppu::PPUDATA:   _ppu.read();          break;
        // case ppu::OAMDATA:   _ppu.readOAMDATA();   break;
        // case ppu::PPUDATA:   _ppu.readPPUDATA();   break;
    }

    buffer = _ppu._reg_DBB;
}

void PictureProcessingUnit::Registers::write(uint16_t address, uint8_t data)
{
    _ppu._reg_DBB = data;

    switch (address) {
        case ppu::PPUCTRL:   _ppu.writePPUCTRL();   break;
        case ppu::PPUMASK:   _ppu.writePPUMASK();   break;
        case ppu::OAMADDR:   _ppu.writeOAMADDR();   break;
        case ppu::OAMDATA:   _ppu.writeOAM();       break;
        // case ppu::OAMDATA:   _ppu.writeOAMDATA();   break;
        case ppu::PPUSCROLL: _ppu.writePPUSCROLL(); break;
        case ppu::PPUADDR:   _ppu.writePPUADDR();   break;
        case ppu::PPUDATA:   _ppu.write();          break;
        // case ppu::PPUDATA:   _ppu.writePPUDATA();   break;
    }
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

void PictureProcessingUnit::readPPUSTATUS()
{
    // TODO
    _reg_W = 0;
}

// void PictureProcessingUnit::readOAMDATA()
// {
//     _reg_DBB = _OAM[_reg_OAMADDR++]; // TODO: ++ ?
// }

// void PictureProcessingUnit::readPPUDATA()
// {
//     read(); // TODO: ++ here or in read() ?
// }

void PictureProcessingUnit::writePPUCTRL()
{

}

void PictureProcessingUnit::writePPUMASK()
{

}

void PictureProcessingUnit::writeOAMADDR()
{
    _reg_OAMADDR = _reg_DBB;
}

// void PictureProcessingUnit::writeOAMDATA()
// {
//     _OAM[_reg_OAMADDR] = _reg_DBB;
// }

void PictureProcessingUnit::writePPUSCROLL()
{
    if (_reg_W) { // second write

    } else { // first write

    }

    _reg_W = !_reg_W;
}

void PictureProcessingUnit::writePPUADDR()
{
    if (_reg_W) { // second write
        reg::setLSB(_reg_T, _reg_DBB);
        _reg_V = _reg_T;
    } else { // first write
        reg::setMSB(_reg_T, _reg_DBB);
        _reg_V &= ppu::VramAddressMask;
    }

    _reg_W = !_reg_W;
}

// void PictureProcessingUnit::writePPUDATA()
// {
//     write();
// }

} // namespace tones