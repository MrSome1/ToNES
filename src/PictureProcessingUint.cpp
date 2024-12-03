
#include "PictureProcessingUnit.h"
#include "Register.h"
#include "Log.h"

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

PictureProcessingUnit::PictureProcessingUnit(Bus &vbus, Bus &mbus)
    : Tickable(3) // TODO: frequency depending on type
    , _vbus(vbus)
    , _registers(*this)
    // TODO: Switch video mode
    , _mode(ppu::VideoMode::NTSC)
    , _format(ppu::NTSC)
{
    _reg_frame.reset(_format.frameCount);
    _reg_line.reset(_format.lineEnd + 1);
    _reg_dot.reset(_format.dotEnd + 1);

    _reg_CX.reset(ppu::TilesOnWidth);
    _reg_CY.reset(ppu::TilesOnHeight);
    _reg_FX.reset(ppu::TileSize);
    _reg_FY.reset(ppu::TileSize);

    _registers.attach(mbus);
    _palettes.attach(_vbus);
}

void PictureProcessingUnit::reset()
{
    /* TODO */
}

void PictureProcessingUnit::setVideoOut(VideoOut output)
{
    _output = output;
}

void PictureProcessingUnit::setFrameEnd(FrameEnd flush)
{
    _flush = flush;
}

void PictureProcessingUnit::_tick()
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

    if (_flush) {
        _flush();
    }
}

void PictureProcessingUnit::dotRender()
{
    if (_reg_dot == _format.dotIdle)
        return;

    renderPixel();
    fetchBackground();
    scrollHorizontal();
    scrollVertical();
}

void PictureProcessingUnit::dotSprite()
{
    // Something wrong here, it can render a whole
    // frame without the syncHorizontal()
    syncHorizontal();
    fetchSprite();
}

void PictureProcessingUnit::dotTile()
{
    fetchBackground();
    scrollHorizontal();
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
    switch (_reg_dot & ppu::TileMask) {
        case 1: // Name table byte
            _reg_V  = (_reg_YX << 10) | (_reg_CY << 5) | _reg_CX;
            _reg_V |= ppu::NameTableBase;
            break;
        case 2:
            read();
            _reg_NTB = _reg_DBB;
            break;
        case 3: // Attribute table byte
            _reg_V  = (_reg_YX << 10) | ((_reg_CY >> 2) << 3) | (_reg_CX >> 2);
            _reg_V |= ppu::NameTableBase | ppu::NameTableSize;
            break;
        case 4:
            read();
            _reg_ATB = _reg_DBB >> (((_reg_CY & 0x02) << 1 )| (_reg_CX & 0x02));
            break;
        case 5: // Pattern table tile low
            _reg_V  = _reg_NTB << 4 | _reg_FY;
            _reg_V |= getPatternTable(ppu::ControllerBit::B);
            break;
        case 6:
            read();
            _reg_BGLB = _reg_DBB;
            break;
        case 7: // Pattern table tile hight
            _reg_V += ppu::TileSize; // TODO: Why not just +1
            break;
        case 0:
            read();
            _reg_BGHB = _reg_DBB;
            break;
    }
}

void PictureProcessingUnit::fetchSprite()
{
    switch (_reg_dot & ppu::TileMask) {
        case 1: // Garbage name table byte
            /* TODO: Addr */
            break;
        case 2:
            read();
            /* TODO */
            break;
        case 3: // Garbage name table byte
            /* TODO: Addr */
            break;
        case 4:
            read();
            /* TODO */
            break;
        case 5: // Pattern table tile low
            /* TODO: Addr */
            break;
        case 6:
            read();
            /* TODO */
            break;
        case 7: // Pattern table tile hight
            /* TODO: Addr */
            break;
        case 0:
            read();
            /* TODO */
            break;
    }
}

void PictureProcessingUnit::renderPixel()
{
    static int32_t mockColor = 0;

    // Outputs of multiplexers
    uint8_t pixelColor;
    uint8_t spriteColor;
    uint8_t backgroundColor;

    // Background color multiplexer
    backgroundColor = (_reg_AT << 2) |
                      GET_BIT(_reg_BGH, _reg_FX) << 1 |
                      GET_BIT(_reg_BGL, _reg_FX);

    // TODO: Sprite color multiplexer
    spriteColor;

    // Prioriy multiplexer
    if (backgroundColor & ppu::ColorIndexMask) {
        if (spriteColor & ppu::ColorIndexMask) {
            if (1) // TODO: priority
                pixelColor = backgroundColor;
            else
                pixelColor = 0x10 | spriteColor;
        } else {
            pixelColor = backgroundColor;
        }
    } else {
        if (spriteColor & ppu::ColorIndexMask)
            pixelColor = 0x10 | spriteColor;
        else
            pixelColor = 0x00;
    }

    _reg_V = ppu::Palettes::PalettesUpperBound | pixelColor;
    read();

    if (_output) {
        // TODO: Output the color in _reg_DBB
        // _output(_reg_CX << 3 | _reg_FX, _reg_CY << 3 | _reg_FY,
        //         std::make_tuple(_reg_DBB, _reg_DBB, _reg_DBB));

        uint16_t x = _reg_CX << 3 | _reg_FX;
        uint16_t y = _reg_CY << 3 | _reg_FY;

        if (x >= ppu::PictureWidth) {
            LOG_DEBUG() << std::hex
                        << " CX: " << _reg_CX.value
                        << " FX: " << _reg_FX.value
                        << std::dec;
        }

        if (y >= ppu::PictureHeight) {
            LOG_DEBUG() << std::hex
                        << " CY: " << _reg_CY.value
                        << " FY: " << _reg_FY.value
                        << std::dec;
        }

        uint8_t r = (mockColor >> 16) & 0xff;
        uint8_t g = (mockColor >>  8) & 0xff;
        uint8_t b = mockColor & 0xff;
        ++mockColor;

        _output(x, y, std::make_tuple(r, g, b));
    }
}

} // namespace tones