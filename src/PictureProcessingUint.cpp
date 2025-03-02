
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

MemoryMap::MemoryMap(PictureProcessingUnit &ppu) : _ppu(ppu) {}

bool MemoryMap::contains(uint16_t addr) const
{
    return addr >= PpuMmioLowerBound && addr < PpuMmioUpperBound;
}

void MemoryMap::read(uint16_t address, uint8_t &buffer) const
{
    switch (address) {
        case ppu::PPUSTATUS: _ppu.readPPUSTATUS(); break;
        case ppu::OAMDATA:   _ppu.readOAMDATA();   break;
        case ppu::PPUDATA:   _ppu.readPPUDATA();   break;
    }

    buffer = _ppu._reg_DBB;
}

void MemoryMap::write(uint16_t address, uint8_t data)
{
    _ppu._reg_DBB = data;

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

void Palettes::dump(std::array <uint8_t, PalettesSize> &colors) const
{
    std::copy(_memory.begin(), _memory.end(), colors.begin());
}

} // namespace ppu

/* PictureProcessingUnit */

PictureProcessingUnit::PictureProcessingUnit(Bus &vbus, Bus &mbus)
    : _vbus(vbus)
    , _mmio(*this)
    // TODO: Switch video mode
    , _mode(ppu::VideoMode::NTSC)
    , _format(ppu::NTSC)
{
    _reg_frame.reset(_format.frameCount);
    _reg_line.reset(_format.lineEnd + 1);
    _reg_dot.reset(_format.dotEnd + 1);

    _mmio.attach(mbus);
    _palettes.attach(_vbus);
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

void PictureProcessingUnit::reset()
{
    _reg_CTRL    = 0x00;
    _reg_MASK    = 0x00;
    _reg_STATUS  = 0x80; // 1??x xxxx
    _reg_OAMADDR = 0x00;

    _reg_V = 0x00;
    _reg_T = 0x00;
    _reg_X = 0x00;
    _reg_W = 0x00;

    _reg_DBB = 0x00;
}

void PictureProcessingUnit::setBlankHandler(VBlank handler)
{
    _handler = handler;
}

void PictureProcessingUnit::setVideoOut(VideoOut output)
{
    _output = output;
}

void PictureProcessingUnit::setFrameEnd(FrameEnd flush)
{
    _flush = flush;
}

void PictureProcessingUnit::dump(Registers_t &registers) const
{
    registers.T = _reg_T;
    registers.V = _reg_V;
    registers.X = _reg_X;
    registers.W = _reg_W;
    registers.CTRL = _reg_CTRL;
    registers.MASK = _reg_MASK;
}

void PictureProcessingUnit::dumpPalettes(
        std::array<uint8_t, ppu::Palettes::PalettesSize> &colors)
{
    _palettes.dump(colors);
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
    _reg_AB = _reg_V;
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
        _fx_mask = 0x8000 >> _reg_X;
        _fx_shift = 7 - _reg_X;
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
        _reg_T &= ppu::VramAddressMask; // T: Z...... ........ <- 0
    }

    _reg_W = !_reg_W;
}

void PictureProcessingUnit::writePPUDATA()
{
    _reg_AB = _reg_V;
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
    if (!showBackground() && !showSprites())
        return;

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
    if (!showBackground() && !showSprites())
        return;

    if (_reg_dot < _format.dotSprite) {
        renderPixel();
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
    if (GET_BIT(_reg_CTRL, ppu::ControllerBit::V) && _handler) {
        // TODO: NMI
        _handler();
    }

    if (_flush) {
        _flush();
    }
}

void PictureProcessingUnit::dotRender()
{
    if (_reg_dot == _format.dotIdle)
        return;

    fetchBackground();
    scrollVertical();
}

void PictureProcessingUnit::dotSprite()
{
    syncHorizontal();
    fetchSprite();
}

void PictureProcessingUnit::dotTile()
{
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
    if ((_reg_V & 0x001F) == 31) { // if coarse X == 31
        _reg_V &= ~0x001F;         // coarse X = 0
        _reg_V ^= 0x0400;          // switch horizontal nametable
    } else {
        _reg_V += 1;               // increment coarse X
    }
}

void PictureProcessingUnit::scrollVertical()
{
    if (_reg_dot != ppu::PictureWidth)
        return;

    if ((_reg_V & 0x7000) != 0x7000) {          // if fine Y < 7
        _reg_V += 0x1000;                       // increment fine Y
    } else {
        _reg_V &= ~0x7000;                      // fine Y = 0
        int y = (_reg_V & 0x03E0) >> 5;         // let y = coarse Y
        if (y == 29) {
            y = 0;                              // coarse Y = 0
            _reg_V ^= 0x0800;                   // switch _reg_Vertical nametable
        } else if (y == 31) {
            y = 0;                              // coarse Y = 0, nametable not switched
        } else {
            y += 1;                             // increment coarse Y
        }
        _reg_V = (_reg_V & ~0x03E0) | (y << 5); // put coarse Y back into _reg_V
    }
}

void PictureProcessingUnit::fetchBackground()
{
    _reg_BGL <<= 1;
    _reg_BGH <<= 1;

    switch (_reg_dot & ppu::TileMask) {
        case 2: // name table byte
            _reg_AB = _reg_V & 0xfff | ppu::VramAddressMask;
            read();
            _reg_NTB = _reg_DBB;
            break;
        case 4: // attribute table byte
            _reg_AB = (_reg_V & 0x0C00) | ((_reg_V >> 4) & 0x38) | ((_reg_V >> 2) & 0x07);
            _reg_AB |= ppu::NameTableBase | ppu::NameTableSize;
            read();
            _reg_ATB = _reg_DBB >> (((_reg_V & 0x40) >> 4 ) | (_reg_V & 0x02));
            _reg_ATB &= ppu::ColorIndexMask;
            break;
        case 6: // pattern table lower byte
            _reg_AB = ((uint16_t)_reg_NTB << 4) | ((_reg_V & 0x7000) >> 12);
            _reg_AB |= getPatternTable(ppu::ControllerBit::B);
            read();
            _reg_BGLB = _reg_DBB;
            break;
        case 0: // pattern table higher byte
            _reg_AB = ((uint16_t)_reg_NTB << 4) | ((_reg_V & 0x7000) >> 12) | ppu::TileSize;
            _reg_AB |= getPatternTable(ppu::ControllerBit::B);
            read();
            _reg_BGHB = _reg_DBB;

            _reg_AT >>= 8;
            copyBackground();
            scrollHorizontal();
            break;
        default:
            break;
    }
}

void PictureProcessingUnit::fetchSprite()
{
    return; // TODO
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
    // Background color
    uint16_t bgColor = (_reg_BGH & _fx_mask) >> _fx_shift;
    bgColor |= (_reg_BGL & _fx_mask) >> _fx_shift >> 1;
    bgColor >>= 7;
    bgColor |= ((_reg_AT & 0x03) << 2);

    // TODO: Sprite color
    uint8_t spColor = 0x10 | 0x00;

    // Prioriy multiplexer
    uint16_t pixelColor;
    if (bgColor & ppu::ColorIndexMask) {
        if (spColor & ppu::ColorIndexMask) {
            pixelColor = 1 ? bgColor : spColor;  // TODO: priority
        } else {
            pixelColor = bgColor;
        }
    } else {
        // TODO: Or just pixelColor = spColor ?
        pixelColor = spColor & ppu::ColorIndexMask ? spColor : 0x00;
    }

    _reg_AB = ppu::Palettes::PalettesLowerBound | pixelColor;
    read();

    if (_output) {
        _output(_reg_dot.value, _reg_line, ppu::Colors[_reg_DBB & ppu::ColorPaletteMask]);
    }
}

inline void PictureProcessingUnit::read()
{
    _vbus.read(_reg_AB & ppu::VramAddressMask, _reg_DBB);
}

inline void PictureProcessingUnit::write()
{
    _vbus.write(_reg_AB & ppu::VramAddressMask, _reg_DBB);
}

inline void PictureProcessingUnit::next()
{
    _reg_V += GET_BIT(_reg_CTRL, ppu::ControllerBit::I) ? 0x20 : 0x01;
}

inline void PictureProcessingUnit::readOAM()
{
    _reg_DBB = _OAM[_reg_OAMADDR];
}

inline void PictureProcessingUnit::writeOAM()
{
    _OAM[_reg_OAMADDR] = _reg_DBB;
}

inline bool PictureProcessingUnit::showBackground()
{
    return GET_BIT(_reg_MASK, ppu::MaskBit::b);
}

inline bool PictureProcessingUnit::showSprites()
{
    return GET_BIT(_reg_MASK, ppu::MaskBit::s);
}

inline void PictureProcessingUnit::copyBackground()
{
    reg::setLSB(_reg_BGL, _reg_BGLB);
    reg::setLSB(_reg_BGH, _reg_BGHB);
    reg::setMSB(_reg_AT, _reg_ATB);
}

inline void PictureProcessingUnit::copyHorizontal()
{
    // v: ....A.. ...BCDEF <- t: ....A.. ...BCDEF
    _reg_V &= ~0x041f;
    _reg_V |= _reg_T & 0x041f;
}

inline void PictureProcessingUnit::copyVertical()
{
    // v: GHIA.BC DEF..... <- t: GHIA.BC DEF.....
    _reg_V &= ~0x7be0;
    _reg_V |= _reg_T & 0x7be0;
}

inline uint16_t PictureProcessingUnit::getPatternTable(ppu::ControllerBit bit)
{
    return GET_BIT(_reg_CTRL, bit) ? PatternTables::TableUpperBankBase:
                                     PatternTables::TableLowerBankBase;
}

} // namespace tones
