
#include "PictureProcessingUnit.h"
#include "Register.h"

namespace tones {
namespace ppu {

void Controller::set(uint8_t value) {
    I = value & 0x04; // 0000 0100
    S = value & 0x08; // 0000 1000
    B = value & 0x10; // 0001 0000
    H = value & 0x20; // 0010 0000
    P = value & 0x40; // 0100 0000
    V = value & 0x80; // 1000 0000
}

void Mask::set(uint8_t value) {
    g = value & 0x01; // 0000 0001
    m = value & 0x02; // 0000 0010
    M = value & 0x04; // 0000 0100
    b = value & 0x08; // 0000 1000
    s = value & 0x10; // 0001 0000
    R = value & 0x20; // 0010 0000
    G = value & 0x40; // 0100 0000
    B = value & 0x80; // 1000 0000
}

void Status::set(uint8_t value) {
    O = value & 0x20; // 0010 0000
    S = value & 0x40; // 0100 0000
    V = value & 0x80; // 1000 0000
}


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
        case ppu::OAMDATA:   _ppu.readOAMDATA();   break;
        case ppu::PPUDATA:   _ppu.readPPUDATA();   break;
    }

    buffer = _ppu._reg_DL;
}

void PictureProcessingUnit::Registers::write(uint16_t address, uint8_t data)
{
    _ppu._reg_DL = data;

    switch (address) {
        case ppu::PPUCTRL:   _ppu.writePPUCTRL();   break;
        case ppu::PPUMASK:   _ppu.writePPUMASK();   break;
        case ppu::OAMADDR:   _ppu.writeOAMADDR();   break;
        case ppu::OAMDATA:   _ppu.writeOAMDATA();   break;
        case ppu::PPUSCROLL: _ppu.writePPUSCROLL(); break;
        case ppu::PPUADDR:   _ppu.writePPUADDR();   break;
        case ppu::PPUDATA:   _ppu.writePPUDATA();   break;
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

void PictureProcessingUnit::readOAMDATA()
{
    /* TODO */
}

void PictureProcessingUnit::readPPUDATA()
{
    read();
    // TODO
}

void PictureProcessingUnit::writePPUCTRL()
{

}

void PictureProcessingUnit::writePPUMASK()
{

}

void PictureProcessingUnit::writeOAMADDR()
{

}

void PictureProcessingUnit::writeOAMDATA()
{

}

void PictureProcessingUnit::writePPUSCROLL()
{
    if (_reg_W) { // second write

    } else { // first write

    }

    ~_reg_W;
}

void PictureProcessingUnit::writePPUADDR()
{
    if (_reg_W) { // second write
        reg::setLSB(_reg_T, _reg_DL);
        _reg_V = _reg_T;
    } else { // first write
        reg::setMSB(_reg_T, _reg_DL);
        _reg_V &= ppu::VramAddressMask;
    }

    ~_reg_W;
}

void PictureProcessingUnit::writePPUDATA()
{

}

} // namespace tones