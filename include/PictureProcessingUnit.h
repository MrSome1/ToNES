#ifndef _TONES_PICTUREPROCESSINGUNIT_H_
#define _TONES_PICTUREPROCESSINGUNIT_H_

#include "Device.h"

namespace tones {
namespace ppu {

const uint16_t RegisterAddressMask = 0x07; // 0000 0111
const uint16_t VramAddressMask = 0x3fff;   // 0011 1111 1111 1111

/* MMIO Register */
typedef enum Register {
    PPUCTRL     = 0x2000,
    PPUMASK,   // 0x2001
    PPUSTATUS, // 0x2002
    OAMADDR,   // 0x2003
    OAMDATA,   // 0x2004
    PPUSCROLL, // 0x2005
    PPUADDR,   // 0x2006
    PPUDATA,   // 0x2007
} Register_t;

typedef struct Controller {
    bool I; /* VRAM address increment per CPU read/write of PPUDATA
               (0: add 1, going across; 1: add 32, going down) */
    bool S; /* Sprite pattern table address for 8x8 sprites
               (0: $0000; 1: $1000; ignored in 8x16 mode) */
    bool B; /* Background pattern table address
               (0: $0000; 1: $1000) */
    bool H; /* Sprite size
               (0: 8x8 pixels; 1: 8x16 pixels) */
    bool P; /* PPU master/slave select
               (0: read backdrop from EXT pins; 1: output color on EXT pins) */
    bool V; /* Generate an NMI at the start of the  vertical blanking interval
               (0: off; 1: on)*/

    void set(uint8_t value);
} Controller_t;

typedef struct Mask {
    bool g; // Greyscale (0: normal color, 1: produce a greyscale display)
    bool m; // 1: Show background in leftmost 8 pixels of screen, 0: Hide
    bool M; // 1: Show sprites in leftmost 8 pixels of screen, 0: Hide
    bool b; // 1: Show background
    bool s; // 1: Show sprites
    bool R; // Emphasize red (green on PAL/Dendy)
    bool G; // Emphasize green (red on PAL/Dendy)
    bool B; // Emphasize blue

    void set(uint8_t value);
} Mask_t;

typedef struct Status {
    bool O; // Sprite overflow
    bool S; // Sprite 0 hit
    bool V; // 0: not in vblank; 1: in vblank

    void set(uint8_t value);
} Status_t;

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

    void tick();

protected:

    //! Read one byte from VRAM
    inline void read() { _vbus.read(_reg_V, _reg_DL); }

    //! Write one byte to VRAM
    inline void write() { _vbus.write(_reg_V, _reg_DL); }

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
    ppu::Controller_t _reg_CTRL;
    ppu::Mask_t _reg_MASK;
    ppu::Status_t reg_STATUS;
    Registers _registers;

    /* Internal Registers */
    uint8_t _reg_V;  // VRAM address / scroll position
    uint8_t _reg_T;  // buffer of register V
    uint8_t _reg_X;  // fine-x position
    uint8_t _reg_W;  // write toggle
    uint8_t _reg_DL; // data latch
};

} // namespace tones

#endif // _TONES_PICTUREPROCESSINGUNIT_H_