#ifndef _TONES_PICTUREPROCESSINGUNIT_H_
#define _TONES_PICTUREPROCESSINGUNIT_H_

#include <functional>
#include <tuple>

#include "Bus.h"
#include "Clock.h"
#include "Device.h"
#include "Register.h"

namespace tones {

typedef std::function<void(void)> VBlank;

typedef std::function<void(void)> FrameEnd;

typedef std::function<void(int x, int y, uint32_t color)> VideoOut;

class PictureProcessingUnit;

namespace ppu {

const int VramAddressMask  = 0x3fff; // 0011 1111 1111 1111
const int SpriteMemorySize = 0x0100; // 256

const int TileSize = 0x08;
const int TileMask = 0x07;
const int TilesOnWidth  = 0x20;   // 32
const int TilesOnHeight = 0x1e;   // 30
const int PictureWidth  = 0x0100; // 256
const int PictureHeight = 0x00f0; // 240

const int NameTableBase = 0x2000;
const int NameTableSize = 0x03c0; // 960

const int ColorIndexMask   = 0x03;
const int ColorPaletteMask = 0x3f;
extern const uint32_t Colors[64];

/* MMIO Register */
typedef enum Register {
    PPUCTRL    = 0x2000,
    PPUMASK    = 0x2001,
    PPUSTATUS  = 0x2002,
    OAMADDR    = 0x2003,
    OAMDATA    = 0x2004,
    PPUSCROLL  = 0x2005,
    PPUADDR    = 0x2006,
    PPUDATA    = 0x2007,
} Register_t;

/**
 * @brief PPUCTRL
 * 
 * Bit Map:
 *   7  6  5  4  3  2  1  0
 *   V  P  H  B  S  I  N  N
 */
enum class ControllerBit {
    V = 0x01 << 7, /* Generate an NMI at the start of the  vertical blanking interval
                      (0: off; 1: on)*/
    P = 0x01 << 6, /* PPU master/slave select
                      (0: read backdrop from EXT pins; 1: output color on EXT pins) */
    H = 0x01 << 5, /* Sprite size
                      (0: 8x8 pixels; 1: 8x16 pixels) */
    B = 0x01 << 4, /* Background pattern table address
                      (0: $0000; 1: $1000) */
    S = 0x01 << 3, /* Sprite pattern table address for 8x8 sprites
                      (0: $0000; 1: $1000; ignored in 8x16 mode) */
    I = 0x01 << 2, /* VRAM address increment per CPU read/write of PPUDATA
                      (0: add 1, going across; 1: add 32, going down) */
};

/**
 * @brief PPUMASK
 * 
 * Bit Map:
 *   7  6  5  4  3  2  1  0
 *   B  G  R  s  b  M  m  g
 */
enum class MaskBit {
    B = 0x01 << 7, // Emphasize blue
    G = 0x01 << 6, // Emphasize green (red on PAL/Dendy)
    R = 0x01 << 5, // Emphasize red (green on PAL/Dendy)
    s = 0x01 << 4, // 1: Show sprites
    b = 0x01 << 3, // 1: Show background
    M = 0x01 << 2, // 1: Show sprites in leftmost 8 pixels of screen, 0: Hide
    m = 0x01 << 1, // 1: Show background in leftmost 8 pixels of screen, 0: Hide
    g = 0x01       // Greyscale (0: normal color, 1: produce a greyscale display)
};

/**
 * @brief PPUSTATUS
 * 
 * Bit Map:
 *   7  6  5  4  3  2  1  0
 *   V  S  O  .  .  .  .  .
 */
enum class StatusBit {
    V = 0x01 << 7, // 0: not in vblank; 1: in vblank
    S = 0x01 << 6, // Sprite 0 hit
    O = 0x01 << 5, // Sprite overflow
};

/**
 * @brief MMIO of PPU Registers
 * 
 *  Access PPU registers in CPU addressing space
 */
class MemoryMap : public Accessible
{

public:

    MemoryMap(tones::PictureProcessingUnit &ppu);

    bool contains(uint16_t addr) const override;

    void read(uint16_t address, uint8_t &buffer) const override;

    void write(uint16_t address, uint8_t data) override;

private:

    tones::PictureProcessingUnit &_ppu;
};

/**
 * @brief Color Palettes
 */
class Palettes : public Accessible
{

public:

    static const int PalettesSize = 0x0020; 
    static const int PalettesMask = 0x001f;
    static const int PalettesLowerBound = 0x3f00; 
    static const int PalettesUpperBound = 0x4000; 

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
 * 
 * Only cares about the picture region, without
 * the borders
 */
class PictureProcessingUnit : public Tickable
{

public:

    /** Constructor
     * @param vbus bus of the ppu
     * @param mbus bus of the cpu
     */
    PictureProcessingUnit(Bus &vbus, Bus &mbus);

    void tick() override;

    void reset();

    void setBlankHandler(VBlank handler);

    void setVideoOut(VideoOut output);

    void setFrameEnd(FrameEnd flush);

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

    void scrollHorizontal();

    void scrollVertical();

    void fetchBackground();

    void fetchSprite();

    void renderPixel();

    /* Helper Functions */

    inline bool showBackground();

    inline bool showSprites();

    inline void copyBackground();

    inline void copyHorizontal();

    inline void copyVertical();

    inline uint16_t getPatternTable(ppu::ControllerBit bit);

private:

    friend class ppu::MemoryMap;

    Bus &_vbus;

    ppu::Palettes _palettes;

    ppu::VideoMode_t _mode;
    ppu::FrameFormat_t _format;

    /* MMIO Registers */
    reg::Bitwise_t _reg_CTRL;
    reg::Bitwise_t _reg_MASK;
    reg::Bitwise_t _reg_STATUS;
    uint8_t        _reg_OAMADDR;

    ppu::MemoryMap _mmio;

    /* Internal Registers */
    uint16_t _reg_V;   // VRAM address / scroll position
    uint16_t _reg_T;   // buffer of register V
    uint8_t  _reg_X;   // fine-x position
    uint8_t  _reg_W;   // write toggle

    /* Render Registers */
    uint16_t _reg_AT;  // for attribute table
    uint16_t _reg_BGL; // for background tile LSB
    uint16_t _reg_BGH; // for background tile MSB
    uint16_t _fx_mask;
    uint16_t _fx_shift;

    /* Buffers */
    uint16_t _reg_AB;  // adress buffer
    uint8_t _reg_DBB;  // data bus buffer
    uint8_t _reg_NTB;  // for name table
    uint8_t _reg_ATB;  // for attribute table
    uint8_t _reg_BGLB; // for background tile LSB
    uint8_t _reg_BGHB; // for background tile MSB

    /* Counters */
    reg::Cycle_t _reg_frame; // index of current frame
    reg::Cycle_t _reg_line;  // index of current scanline
    reg::Cycle_t _reg_dot;   // index of current pixel

    /* Object Attribute Memory */
    uint8_t _OAM[ppu::SpriteMemorySize];

    /* Callbacks */

    VBlank _handler;

    VideoOut _output;

    FrameEnd _flush;
};

} // namespace tones

#endif // _TONES_PICTUREPROCESSINGUNIT_H_