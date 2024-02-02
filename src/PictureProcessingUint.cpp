
#include "PictureProcessingUnit.h"
#include "Register.h"

namespace tones {

namespace ppu {

/* Registers */

const FrameFormat_t NTSC = {
    60,
    261, 0, 240, 241, 261,
    0, 1, 257, 321, 337, 340,
};

const FrameFormat_t PAL = {
    0, // TODO
};

const FrameFormat_t Dendy = {
    0, // TODO
};

Registers::Registers(PictureProcessingUnit &ppu) : _ppu(ppu) {}

bool Registers::contains(uint16_t addr) const
{
    return addr >= PpuMmioLowerBound && addr < PpuMmioUpperBound;
}

void Registers::read(uint16_t address, uint8_t &buffer) const
{
    switch (address & PpuMmioAddressMask) {
        case ppu::PPUSTATUS: _ppu.readPPUSTATUS(); break;
        case ppu::OAMDATA:   _ppu.readOAMDATA();   break;
        case ppu::PPUDATA:   _ppu.readPPUDATA();   break;
    }

    buffer = _ppu._reg_DBB;
}

void Registers::write(uint16_t address, uint8_t data)
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

/* Palettes */

Palettes::Palettes() {}

bool Palettes::contains(uint16_t addr) const
{
    return addr > PalettesLowerBound && addr < PalettesUpperBound;
}

void Palettes::read(uint16_t address, uint8_t &buffer) const
{
    buffer = _memory[address & PalettesMask];
}

void Palettes::write(uint16_t address, uint8_t data)
{
    _memory[address & PalettesMask] = data;
}

} // namespace ppu

/* PictureProcessingUnit */

PictureProcessingUnit::PictureProcessingUnit(Bus &vbus)
    : _vbus(vbus)
    , _registers(*this)
    // TODO: Switch video mode
    , _mode(ppu::VideoMode::NTSC)
    , _format(ppu::NTSC)
{
    _reg_frame.reset(_format.frameCount - 1);
    _reg_line.reset(_format.lineEnd);
    _reg_dot.reset(_format.dotEnd);

    _reg_CX.reset(ppu::TilesOnWidth - 1);
    _reg_CY.reset(ppu::TilesOnHeight - 1);
    _reg_FX.reset(ppu::TileSize - 1);
    _reg_FY.reset(ppu::TileSize - 1);

    _palettes.attach(_vbus);
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
    if (_reg_line < _format.linePost) {
        lineRender();
    } else if (_reg_line == _format.linePre) {
        linePre();
    } else if (_reg_line == _format.lineVBlank) {
        lineVBlank();
    } // Just idles for other scanlines

    forward();
}

void PictureProcessingUnit::readPPUSTATUS()
{
    _reg_DBB = _reg_STATUS;
    CLR_BIT(_reg_STATUS, ppu::StatusBit::V);
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

/**
 * Registers T and V are composed this way during rendering
 * 
 *   yyy NN YYYYY XXXXX
 *   ||| || ||||| +++++-- coarse X scroll
 *   ||| || +++++-------- coarse Y scroll
 *   ||| ++-------------- nametable select
 *   +++----------------- fine Y scroll
 */
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
        // _reg_V &= ppu::VramAddressMask; // T: Z...... ........ <- 0
    }

    _reg_W = !_reg_W;
}

void PictureProcessingUnit::writePPUDATA()
{
    write();
    next();
}

void PictureProcessingUnit::forward()
{
    if (_reg_line.full())
        ++_reg_frame;
    
    if (_reg_dot.full())
        ++_reg_line;

    ++_reg_dot;    
}

void PictureProcessingUnit::linePre()
{
    if (_reg_dot < _format.dotSprite) {
        clearStatus();
        dotRender();
    } else if (_reg_dot < _format.dotTile) {
        syncVertical();
        dotSprite();
    } else if (_reg_dot < _format.dotFetch) {
        dotTile();
    } else {
        dotFetch();
        skipIdleDot();
    }
}

void PictureProcessingUnit::lineRender()
{
    if (_reg_dot < _format.dotSprite) {
        dotRender();
    } else if (_reg_dot < _format.dotTile) {
        dotSprite();
    } else if (_reg_dot < _format.dotFetch) {
        dotTile();
    } else {
        dotFetch();
    }
}

void PictureProcessingUnit::lineVBlank()
{
    if (_reg_dot != _format.dotRender)
        return;

    SEL_BIT(_reg_STATUS, ppu::StatusBit::V);
    if (GET_BIT(_reg_CTRL, ppu::ControllerBit::V)) {
        // TODO: NMI
    }
}

void PictureProcessingUnit::dotRender()
{
    if (_reg_dot == _format.dotIdle)
        return;

    fetchBackground();
    renderPixel();

    scrollHorizontal();
    scrollVertical();
}

void PictureProcessingUnit::dotSprite()
{
    syncHorizontal();
    fetchSprite();
}

void PictureProcessingUnit::dotTile()
{
    scrollHorizontal();
    fetchBackground();
}

void PictureProcessingUnit::dotFetch()
{
    /* TODO */
}

void PictureProcessingUnit::clearStatus()
{
    if (_reg_dot.value == _format.dotRender)
        _reg_STATUS = 0x00; // clear status
}

void PictureProcessingUnit::skipIdleDot()
{
    if (_reg_dot.full() && reg::isOdd(_reg_frame.value) &&
        showBackground() && showSprites()) { // TODO: only BG?
        forward(); // skip the next idle dot for odd frames
    }
}

void PictureProcessingUnit::syncHorizontal()
{
    if (_reg_dot == _format.dotSprite)
        copyHorizontal();
}

void PictureProcessingUnit::syncVertical()
{
    // TODO: Actually in dot 280~340
    if (_reg_dot == _format.dotSprite)
        copyVertical();
}

void PictureProcessingUnit::scrollHorizontal()
{
    if (_reg_dot & ppu::TileMask)
        return;

    if (_reg_FX.full()) {
        if (_reg_CX.full())
            _reg_YX ^= 0b01;
        ++_reg_CX;
    }
    ++_reg_FX;

    copyRender(); // TODO
}

void PictureProcessingUnit::scrollVertical()
{
    if (_reg_dot != ppu::PictureWidth)
        return;

    if (_reg_FY.full()) {
        if (_reg_CY.full())
            _reg_YX ^= 0b10;
        ++_reg_CY;
    }
    ++_reg_FY;
}

void PictureProcessingUnit::fetchBackground()
{
    switch (_reg_dot & 0x07) {
        case 0: // Name table byte
            _reg_V  = _reg_YX << 10 | _reg_CY << 5 | _reg_CX;
            _reg_V |= VideoRandomAccessMemory::VramLowerBound;
            break;
        case 1:
            read();
            _reg_NTB = _reg_DBB;
            break;
        case 2: // Attribute table byte
            // TODO: Addr
            break;
        case 3:
            read();
            _reg_ATB = _reg_DBB;
            break;
        case 4: // Pattern table tile low
            _reg_V  = _reg_NTB << 4 | _reg_FY;
            _reg_V |= getPatternTable(ppu::ControllerBit::B);
            break;
        case 5:
            read();
            _reg_BGLB = _reg_DBB;
            break;
        case 6: // Pattern table tile hight
            _reg_V += ppu::TileSize; // ???
            break;
        case 7:
            read();
            _reg_BGHB = _reg_DBB;
            break;
    }
}

void PictureProcessingUnit::fetchSprite()
{
    switch (_reg_dot & 0x07) {
        // Garbage name table byte
        case 0: /* TODO: Addr */ break;
        case 1: read(); /* TODO */ break;

        // Garbage name table byte
        case 2: /* TODO: Addr */ break;
        case 3: read(); /* TODO */ break;

        // Pattern table tile low
        case 4: /* TODO: Addr */ break;
        case 5: read(); /* TODO */ break;

        // Pattern table tile hight
        case 6: /* TODO: Addr */ break;
        case 7: read(); /* TODO */ break;
    }
}

void PictureProcessingUnit::renderPixel()
{
    /* TODO */
}

} // namespace tones