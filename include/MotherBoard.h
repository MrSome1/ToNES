/* Topology Diagram
 *
 *  /--------- 8-Bit Data Bus ----------\
 *  \----^---------^-------^-------^----/
 *       |         |       |       |
 *   +---v---+  +--+--+    |       |
 *   |       |  | ROM | +--v--+ +--v--+
 *   |       |  +--^--+ |     | |     |
 *   |  CPU  |     |    | RAM | | I/O |
 *   |       |  +--+--+ |     | |     |
 *   |       |  | MMC | +--^--+ +--^--+
 *   +---v---+  +--^--+    |       |
 *       |         |       |       |
 *  /----+---------+---------------+----\
 *  \-------- 16-Bit Address Bus -------/
 */

#ifndef _TONES_MOTHERBOARD_H_
#define _TONES_MOTHERBOARD_H_

#include <array>
#include <atomic>
#include <functional>

#include "Bus.h"
#include "Clock.h"
#include "Device.h"
#include "MicroProcessor.h"
#include "PictureProcessingUnit.h"
#include "Cartridge.h"

namespace tones {

const int OAMDMA = 0x4014; // OAM DMA MMIO register address
const int DMCDMA = 0x4015; // DMC DMA MMIO register address

/**
 * @brief The DMA Unit
 *
 */
class DirectMemoryAccess final : public Accessible
{

public:

    DirectMemoryAccess(uint16_t mmio, uint16_t dest, uint16_t len);

    bool contains(uint16_t addr) const override;

    void read(uint16_t address, uint8_t &buffer) const override;

    void write(uint16_t address, uint8_t data) override;

private:

    const uint16_t _mmio; // MMIO register address
    const uint16_t _dest; // address to copy to
    const uint16_t _len;  // bytes to copy

    uint8_t  _buff;
    uint8_t  _page; // page number to copy from
    uint16_t _addr; // base address to copy from
};

/**
 * @brief A base class to get the output signals
 *
 */
class OutputPanel
{

public:

    /* Real Outputs */

    virtual void onVideoDotRendered(int x, int y, uint32_t color)
    {
        (void)x;
        (void)y;
        (void)color;
    }

    virtual void onVideoFrameRendered() {}

    virtual void onAudioOutput() {}

    /* Debug Infos */

    virtual void onRegistersChanged() {}
};

/**
 * @brief MotherBoard of NES
 * 
 */
class MotherBoard
{

public:

    MotherBoard();

    void insert(CartridgePtr &card);

    void start();

    void stop();

    void pause();

    void resume();

    /* Status */

    bool isStarted() const;

    bool isPaused() const;

    bool isRunning() const;

    /* Debuging */

    void step();

    void reset();

    void dumpCpuRegisters(MicroProcessor::Registers_t &regs);

    void dumpPpuRegisters(PictureProcessingUnit::Registers_t &regs);

    void dumpPpuPalettes(std::array<uint8_t, ppu::Palettes::PalettesSize> &colors);

    /* Callbacks */

    void setOutputPanel(OutputPanel &output);

protected:

    void eject();

    void run();

private:

    uint32_t _frequency;

    /* Hardwares */

    Bus _mbus; // Bus of CPU

    Bus _vbus; // Bus of PPU

    Clock _clock;

    MicroProcessor _cpu;

    PictureProcessingUnit _ppu;

    RandomAccessMemory _pram; // RAM of CPU

    VideoRandomAccessMemory _vram; // RAM of PPU

    // TODO: DMC DMA
    DirectMemoryAccess _odma; // OAM DMA

    CartridgePtr _card;

    /* Flags */

    std::atomic<bool> _started;

    std::atomic<bool> _paused;

    /* Output */

    OutputPanel *_output;
};

} // namespace tones

#endif // _TONES_MOTHERBOARD_H_
