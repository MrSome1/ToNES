#ifndef _TONES_PICTUREPROCESSINGUNIT_H_
#define _TONES_PICTUREPROCESSINGUNIT_H_

#include "Device.h"
#include "Register.h"

namespace tones {

class PictureProcessingUnit;

namespace ppu {

const int VramAddressMask  = 0x3fff; // 0011 1111 1111 1111
const int SpriteMemorySize = 0x0100;  // 256

/* MMIO Register */
typedef enum Register {
    PPUCTRL,   // 0x2000
    PPUMASK,   // 0x2001
    PPUSTATUS, // 0x2002
    OAMADDR,   // 0x2003
    OAMDATA,   // 0x2004
    PPUSCROLL, // 0x2005
    PPUADDR,   // 0x2006
    PPUDATA,   // 0x2007
} Register_t;

/**
 * @brief PPUCTRL
 * 
 * Bit Map:
 *   7  6  5  4  3  2  1  0
 *   V  P  H  B  S  I  N  N
 */
enum class ControllerBit {
    X, // ???
    Y, // ???
    I, /* VRAM address increment per CPU read/write of PPUDATA
          (0: add 1, going across; 1: add 32, going down) */
    S, /* Sprite pattern table address for 8x8 sprites
          (0: $0000; 1: $1000; ignored in 8x16 mode) */
    B, /* Background pattern table address
          (0: $0000; 1: $1000) */
    H, /* Sprite size
          (0: 8x8 pixels; 1: 8x16 pixels) */
    P, /* PPU master/slave select
          (0: read backdrop from EXT pins; 1: output color on EXT pins) */
    V, /* Generate an NMI at the start of the  vertical blanking interval
               (0: off; 1: on)*/
};

/**
 * @brief PPUMASK
 * 
 * Bit Map:
 *   7  6  5  4  3  2  1  0
 *   B  G  R  s  b  M  m  g
 */
enum class MaskBit {
    g, // Greyscale (0: normal color, 1: produce a greyscale display)
    m, // 1: Show background in leftmost 8 pixels of screen, 0: Hide
    M, // 1: Show sprites in leftmost 8 pixels of screen, 0: Hide
    b, // 1: Show background
    s, // 1: Show sprites
    R, // Emphasize red (green on PAL/Dendy)
    G, // Emphasize green (red on PAL/Dendy)
    B, // Emphasize blue
};

/**
 * @brief PPUSTATUS
 * 
 * Bit Map:
 *   7  6  5  4  3  2  1  0
 *   V  S  O  .  .  .  .  .
 */
enum class StatusBit {
    O = 5, // Sprite overflow
    S,     // Sprite 0 hit
    V,     // 0: not in vblank; 1: in vblank
};

/**
 * @brief MMIO Registers
 * 
 *  PPU Registers mapped into CPU addressing space
 */
class Registers : public Device
{

public:

    Registers(tones::PictureProcessingUnit &ppu);

    bool contains(uint16_t addr) const override;

    void read(uint16_t address, uint8_t &buffer) const override;

    void write(uint16_t address, uint8_t data) override;

private:

    tones::PictureProcessingUnit &_ppu;
};

/**
 * @brief Color Palettes
 */
class Palettes : public Device
{

    static const int PalettesSize = 0x0020; 
    static const int PalettesMask = 0x001f;
    static const int PalettesLowerBound = 0x3f00; 
    static const int PalettesUpperBound = 0x4000; 

public:

    Palettes();

    bool contains(uint16_t addr) const override;

    void read(uint16_t address, uint8_t &buffer) const override;

    void write(uint16_t address, uint8_t data) override;

private:

    std::array <uint8_t, PalettesSize> _memory;
};

/* TODO */
typedef enum class VideoMode {
    NTSC,
    PAL,
    Dendy,
} VideoMode_t;

/* TODO */
typedef struct FrameFormat {
    uint16_t frameCount;

    uint16_t linePre;    // pre-render scanline
    uint16_t lineRender; // start of visible scanlines
    uint16_t linePost;   // post-render scanline
    uint16_t lineVBlank; // start of vertical blanking scanlines
    uint16_t lineEnd;

    uint16_t dotIdle;   // an idle cycle
    uint16_t dotRender; // fetches each tile for the current scanline
    uint16_t dotSprite; // fetches tile data for the srites on next scanline
    uint16_t dotTile;   // fetches the first two tiles for the next scanline
    uint16_t dotFetch;  // fetches two bytes for unknown purpose
    uint16_t dotEnd;
} FrameFormat_t;

extern const FrameFormat_t NTSC;
extern const FrameFormat_t PAL;
extern const FrameFormat_t Dendy;

} // namespace ppu

/**
 * @brief PPU
 */
class PictureProcessingUnit
{

public:

    PictureProcessingUnit(Bus &vbus);

    void attach(Bus &bus);

    void reset();

    void tick();

protected:

    //! Next address of VRAM
    void next();

    //! Read one byte from VRAM
    void read();

    //! Write one byte to VRAM
    void write();

    //! Read one byte from OAM
    void readOAM();

    //! Write one byte to OAM
    void writeOAM();

    /* Register Accessing */

    void readPPUSTATUS();

    void readOAMDATA();

    void readPPUDATA();

    void writePPUCTRL();

    void writePPUMASK();

    void writeOAMADDR();

    void writeOAMDATA();

    void writePPUSCROLL();

    void writePPUADDR();

    void writePPUDATA();

    /* Rendering */

    //! Increase the indexes
    void forward();

    //! For pre-render scanline
    void linePre();

    //! For visible scanlines
    void lineRender();

    //! For vertical blanking lines
    void lineVBlank();

    void dotRender();

    void dotSprite();

    void dotTile();

    void dotFetch();

    void clearStatus();

    void skipIdleDot();

    void syncHorizontal();

    void syncVertical();

    void updateHorizontal();

    void updateVertical();

    /* Helper Functions */

    bool showBackground();

    bool showSprites();

    void copyHorizontal();

    void copyVertical();

    void increaseHorizontal();

    void increaseVertical();

private:

    friend class ppu::Registers;

    Bus &_vbus;

    ppu::Palettes _palettes;

    ppu::VideoMode_t _mode;
    ppu::FrameFormat_t _format;

    /* MMIO Registers */
    Bitwise_t _reg_CTRL;
    Bitwise_t _reg_MASK;
    Bitwise_t _reg_STATUS;
    uint8_t   _reg_OAMADDR;
    ppu::Registers _registers;

    /* Internal Registers */
    uint16_t _reg_V;   // VRAM address / scroll position
    uint16_t _reg_T;   // buffer of register V
    uint8_t  _reg_X;   // fine-x position
    uint8_t  _reg_W;   // write toggle
    uint8_t  _reg_DBB; // data bus buffer

    /* Unknown Registers */

    /* Counters */
    reg::Cycle _reg_frame; // index of current frame
    reg::Cycle _reg_line;  // index of current scanline
    reg::Cycle _reg_dot;   // index of current pixel
    reg::Cycle _reg_hori;  // horizontal counter
    reg::Cycle _reg_vert;  // vertical counter

    /* Object Attribute Memory */
    uint8_t _OAM[ppu::SpriteMemorySize];
};

inline void PictureProcessingUnit::read()
{
    _vbus.read(_reg_V, _reg_DBB);
}

inline void PictureProcessingUnit::write()
{
    _vbus.write(_reg_V, _reg_DBB);
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

inline void PictureProcessingUnit::copyHorizontal()
{
    // v: ....A.. ...BCDEF <- t: ....A.. ...BCDEF 
    _reg_V &= 0xfbe0;
    _reg_V |= _reg_T & 0x041f;
}

inline void PictureProcessingUnit::copyVertical()
{
    // v: GHIA.BC DEF..... <- t: GHIA.BC DEF.....
    _reg_V &= 0x041f;
    _reg_V |= _reg_T & 0xfbe0;
}

inline void PictureProcessingUnit::increaseHorizontal()
{
    // TODO: increase horizontal V
}

inline void PictureProcessingUnit::increaseVertical()
{
    // TODO: increase vertical V
}

} // namespace tones

#endif // _TONES_PICTUREPROCESSINGUNIT_H_