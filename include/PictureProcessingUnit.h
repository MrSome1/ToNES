#ifndef _TONES_PICTUREPROCESSINGUNIT_H_
#define _TONES_PICTUREPROCESSINGUNIT_H_

#include "Device.h"

namespace tones {
namespace ppu {

/* MMIO Register */
typedef enum Register {
    PPUCTRL = 0x2000,
    PPUMASK,
    PPUSTATUS,
    OAMADDR,
    OAMDATA,
    PPUSCROLL,
    PPUADDR,
    PPUDATA,
} Register_t;

const int RegisterCount = 8;

} // namespace ppu

/**
 * @brief PPU
 */
class PictureProcessingUnit
{

    /**
     * @brief MMIO Registers
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

    /* Register Accessing */

    void readPPUSTATUS(uint8_t &byte);

    void readOAMDATA(uint8_t &byte);

    void readPPUDATA(uint8_t &byte);

    void writePPUCTRL(uint8_t byte);

    void writePPUMASK(uint8_t byte);

    void writeOAMADDR(uint8_t byte);

    void writeOAMDATA(uint8_t byte);

    void writePPUSCROLL(uint8_t byte);

    void writePPUADDR(uint8_t byte);

    void writePPUDATA(uint8_t byte);

private:

    friend class Registers;

    Bus _vbus;

    Registers _registers;

    /* Internal Registers */
    uint8_t _reg_v;
    uint8_t _reg_t;
    uint8_t _reg_x;
    uint8_t _reg_w;
    uint8_t _reg_DL; // data latch
};

} // namespace tones

#endif // _TONES_PICTUREPROCESSINGUNIT_H_