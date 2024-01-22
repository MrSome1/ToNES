
#include "PictureProcessingUnit.h"
#include "Register.h"

namespace tones {

using namespace ppu;

namespace ppu {

} // namespace ppu

/* Registers */

PictureProcessingUnit::Registers::Registers(PictureProcessingUnit &ppu) : _ppu(ppu) {}

bool PictureProcessingUnit::Registers::contains(uint16_t addr) const
{
    return addr >= PpuMmioLowerBound && addr < PpuMmioUpperBound;
}

void PictureProcessingUnit::Registers::read(uint16_t address, uint8_t &buffer) const
{
    switch (address & PpuMmioAddressMask) {
        case ppu::PPUSTATUS: _ppu.readPPUSTATUS(); break;
        case ppu::OAMDATA:   _ppu.readOAMDATA();   break;
        case ppu::PPUDATA:   _ppu.readPPUDATA();   break;
    }

    buffer = _ppu._reg_DBB;
}

void PictureProcessingUnit::Registers::write(uint16_t address, uint8_t data)
{
    _ppu._reg_DBB = data;

    switch (address & PpuMmioAddressMask) {
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

void PictureProcessingUnit::reset()
{
    /* TODO */
}

void PictureProcessingUnit::tick()
{
    /* TODO */
}

void PictureProcessingUnit::readPPUSTATUS()
{
    _reg_DBB = _reg_STATUS;
    CLR_BIT(_reg_STATUS, StatusBit::V);
    _reg_W = 0;
}

void PictureProcessingUnit::readOAMDATA()
{
    readOAM();
}

void PictureProcessingUnit::readPPUDATA()
{
    read();
    next();
    // TODO: internal read buffer
}

void PictureProcessingUnit::writePPUCTRL()
{
    _reg_CTRL = _reg_DBB;
    _reg_T &= 0xf3ff; // T: ...GH.. ........ <- DBB: ......GH
    _reg_T |= (uint16_t)(_reg_DBB & 0x03) << 10;
}

void PictureProcessingUnit::writePPUMASK()
{
    _reg_MASK = _reg_DBB;
}

void PictureProcessingUnit::writeOAMADDR()
{
    _reg_OAMADDR = _reg_DBB;
}

void PictureProcessingUnit::writeOAMDATA()
{
    writeOAM();
    ++_reg_OAMADDR;
}

void PictureProcessingUnit::writePPUSCROLL()
{
    if (_reg_W) { // second write
        _reg_T &= 0x0c1f;         // T: FGH..AB CDE..... <- DBB: ABCDEFGH
        _reg_T |= (uint16_t)(_reg_DBB & 0x07) << 12;
        _reg_T |= (uint16_t)(_reg_DBB & 0xf8) <<  2;
    } else { // first write
        _reg_T &= 0xffe0;         // T: ....... ...ABCDE <- DBB: ABCDE...
        _reg_T |= _reg_DBB >> 3;
        _reg_X  = _reg_DBB & 0x7; // X:              FGH <- DBB: .....FGH
    }

    _reg_W = !_reg_W;
}

void PictureProcessingUnit::writePPUADDR()
{
    if (_reg_W) { // second write
        reg::setLSB(_reg_T, _reg_DBB);  // T: ....... ABCDEFGH <- DBB: ABCDEFGH
        _reg_V = _reg_T;
    } else { // first write
        reg::setMSB(_reg_T, _reg_DBB);  // T: .CDEFGH ........ <- DBB: ..CDEFGH
        _reg_V &= ppu::VramAddressMask; // T: Z...... ........ <- 0
    }

    _reg_W = !_reg_W;
}

void PictureProcessingUnit::writePPUDATA()
{
    write();
    next();
}

} // namespace tones