#ifndef _TONES_PICTUREPROCESSINGUNIT_H_
#define _TONES_PICTUREPROCESSINGUNIT_H_

#include "Device.h"
#include "Register.h"

namespace tones {
namespace ppu {

const int VramAddressMask  = 0x3fff; // 0011 1111 1111 1111
const int SpriteMemorySize = 0x100;  // 256

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

} // namespace ppu

/**
 * @brief PPU
 */
class PictureProcessingUnit
{

    /**
     * @brief MMIO Register Accessor
     */
    class Registers : public Device
    {

    public:

        Registers(PictureProcessingUnit &ppu);

        bool contains(uint16_t addr) const override;

        void read(uint16_t address, uint8_t &buffer) const override;

        void write(uint16_t address, uint8_t data) override;

    private:

        PictureProcessingUnit &_ppu;
    };

public:

    PictureProcessingUnit(Bus &vbus);

    void attach(Bus &bus);

    void reset();

    void tick();

protected:

    //! Next address of VRAM
    inline void next() {
        _reg_V += GET_BIT(_reg_CTRL, ppu::ControllerBit::I) ? 0x20 : 0x01;
    }

    //! Read one byte from VRAM
    inline void read() { _vbus.read(_reg_V, _reg_DBB); }

    //! Write one byte to VRAM
    inline void write() { _vbus.write(_reg_V, _reg_DBB); } 

    /* OAM Accessing */

    inline void readOAM() { _reg_DBB = _OAM[_reg_OAMADDR]; }

    inline void writeOAM() { _OAM[_reg_OAMADDR] = _reg_DBB; }

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

private:

    friend class Registers;

    Bus &_vbus;

    /* MMIO Registers */
    Bitwise_t _reg_CTRL;
    Bitwise_t _reg_MASK;
    Bitwise_t _reg_STATUS;
    uint8_t   _reg_OAMADDR;
    Registers _registers;

    /* Internal Registers */
    uint16_t _reg_V;   // VRAM address / scroll position
    uint16_t _reg_T;   // buffer of register V
    uint8_t  _reg_X;   // fine-x position
    uint8_t  _reg_W;   // write toggle
    uint8_t  _reg_DBB; // data bus buffer

    /* Object Attribute Memory */
    uint8_t _OAM[ppu::SpriteMemorySize];
};

} // namespace tones

#endif // _TONES_PICTUREPROCESSINGUNIT_H_