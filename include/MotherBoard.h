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
 *   |       |  | MMC | +--+--+ +--^--+
 *   +---v---+  +--^--+            |
 *       |         |               |
 *       +-------------------------+
 *       +--- 16-Bit Address Bus --+
 */

#ifndef _TONES_MOTHERBOARD_H_
#define _TONES_MOTHERBOARD_H_

#include "MicroProcessor.h"
#include "DataBus.h"
#include "Cartridge.h"

namespace tones {

class MotherBoard
{

public:

    MotherBoard();

private:
    DataBus _bus;
    MicroProcessor _cpu;
    CartridgeReader _reader;
};

} // namespace tones

#endif // _TONES_MOTHERBOARD_H_