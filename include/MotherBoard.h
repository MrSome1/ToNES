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

#include "MicroProcessor.h"
#include "Device.h"
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

    void mount(CartridgePtr &card);

    void unmount();

    /* Debug APIs */

    void dumpCPU();

private:

    Bus _bus;

    MicroProcessor _cpu;

    RandomAccessMemory _ram;

    CartridgePtr _card;
};

} // namespace tones

#endif // _TONES_MOTHERBOARD_H_