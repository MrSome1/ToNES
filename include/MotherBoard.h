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

#include "Device.h"
#include "MicroProcessor.h"
#include "PictureProcessingUnit.h"
#include "Cartridge.h"

namespace tones {

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

    void pause();

    void insert(CartridgePtr &card);

    void eject();

    /* Debug APIs */

    void dumpCPU();

private:

    Bus _bus; // Bus of CPU

    Bus _vbus; // Bus of PPU

    MicroProcessor _cpu;

    PictureProcessingUnit _ppu;

    RandomAccessMemory _ram;

    VideoRandomAccessMemory _vram;

    CartridgePtr _card;
};

} // namespace tones

#endif // _TONES_MOTHERBOARD_H_