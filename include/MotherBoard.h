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
#include "DataBus.h"
#include "Cartridge.h"

namespace tones {

/**
 * @brief RAM
 */
class RandomAccessMemory: public Device
{
    static const uint16_t RamBase = 0x0000;
    static const uint16_t RamSize = 0x2000;
    static const uint16_t RamMask = 0x07ff; // for mirroring 4 times

public:

    RandomAccessMemory();

    void read(uint16_t address, uint8_t &buffer) const override;

    void write(uint16_t address, uint8_t data) override;

private:

    std::vector<uint8_t> _memory;
};

/**
 * @brief MotherBoard of NES
 * 
 */
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