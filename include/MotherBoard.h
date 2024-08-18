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

typedef std::function<void(MicroProcessor::Registers_t&)> CpuViewer;

/**
 * @brief MotherBoard of NES
 * 
 */
class MotherBoard
{

public:

    MotherBoard();

    void reset();

    void start();

    void stop();

    void pause();

    void resume();

    void insert(CartridgePtr &card);

    void eject();

    /* Status */

    bool isStarted() const;

    bool isRunning() const;

    /* Callbacks */

    void setVideoOut(VideoOut output);

    void setFrameEnd(FrameEnd flush);

    /* Debug APIs */

    void setCpuViewer(CpuViewer viewer);

protected:

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

    /* Debuggers */

    CpuViewer _cpuViewer;
};

} // namespace tones

#endif // _TONES_MOTHERBOARD_H_