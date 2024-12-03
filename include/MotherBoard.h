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

#include <atomic>
#include <functional>

#include "Clock.h"
#include "Device.h"
#include "MicroProcessor.h"
#include "PictureProcessingUnit.h"
#include "Cartridge.h"

namespace tones {

class MotherBoard;

/**
 * @brief A base class to get the output signals
 *
 */
class OutputPanel
{

public:

    /* Real Outputs */
    virtual void onVideoDotRendered(int x, int y, const RGB &color)
    {
        (void)x;
        (void)y;
        (void)color;
    };

    virtual void onVideoFrameRendered() {};

    virtual void onAudioOutput() {};

    /* Debug Infos */

    virtual void onCpuStepped() {};

protected:

    friend class MotherBoard;

    MicroProcessor::Registers_t _registers;
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

    bool isRunning() const;

    /* Callbacks */

    void setOutputPanel(OutputPanel &output);

protected:

    void reset();

    void eject();

    void run();

    void debugCpu();

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

    CartridgePtr _card;

    /* Flags */

    std::atomic<bool> _started;

    std::atomic<bool> _running;

    /* Output */

    OutputPanel *_output;
};

} // namespace tones

#endif // _TONES_MOTHERBOARD_H_