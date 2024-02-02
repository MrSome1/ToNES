/**
 *  CPU Memory Map                                     PPU Memory Map
 *
 * ------------------------------------- $10000        ------------------------------------- $10000
 *    Cartridge   | PRG-ROM Upper Bank                     Mirrors    | Mirrors $0000-$3FFF
 *                +--------------------- $C000         ---------------+--------------------- $4000
 *     PRG-ROM    | PRG-ROM Lower Bank                                | Mirrors $2000-$2007
 * ---------------+--------------------- $8000                        +--------------------- $3F20
 *      SRAM      | Cartridge Save RAM                     Palettes   | Sprite Palette
 * ---------------+--------------------- $6000                        +--------------------- $3F10
 *  Expansion ROM |                                                   | Image Palette
 * ---------------+--------------------- $4020         ---------------+--------------------- $3F00
 *                | I/O Registers                                     | Mirrors $2000-$2EFF
 *                +--------------------- $4000                        +--------------------- $3000
 *  I/O Registers | Mirrors $2000-$2007                               | Attribute Table 3
 *                +--------------------- $2008                        +--------------------- $2FC0
 *                | I/O Registers                                     | Name Table 3
 * ---------------+--------------------- $2000                        +--------------------- $2C00
 *                | Mirrors $0000-$07FF                               | Attribute Table 2
 *                +--------------------- $0800                        +--------------------- $2BC0
 *                | RAM                                               | Name Table 2
 *       RAM      +--------------------- $0200           Name Tables  +--------------------- $2800
 *                | Stack                                             | Attribute Table 1
 *                +--------------------- $0100                        +--------------------- $27C0
 *                | Zero Page                                         | Name Table 1
 * ------------------------------------- $0000                        +--------------------- $2400
 *                                                                    | Attribute Table 0
 *                                                                    +--------------------- $23C0
 *                                                                    | Name Table 0
 *                                                     ------------------------------------- $2000
 *                                                         Pattern    | Pattern Table 1
 *                                                                    +--------------------- $1000
 *                                                          Tables    | Pattern Table 0
 *                                                     ------------------------------------- $0000
 */


#ifndef _TONES_DEVICE_H_
#define _TONES_DEVICE_H_

#include <cinttypes>
#include <array>
#include <vector>

namespace tones {

class Device;

/**
 * @brief Abstraction of the system bus
 * 
 * This class inclues the Address Bus, Control Bus and
 * Data Bus, actually, and refers to all devices in the
 * system that can be accessed by the CPU
 */
class Bus
{

public:

    Bus();
    ~Bus();

    //! Abstraction of the Address Bus
    uint16_t address() const;

    //! Abstraction of the Data Bus, with Control Bus of read mode
    void read(uint16_t address, uint8_t &buffer);

    //! Abstraction of the Data Bus, with Control Bus of write mode
    void write(uint16_t address, uint8_t data);

protected:

    friend class Device;

    //! Mount a device
    void attach(Device *device);

    //! Unmount a device
    void detach(Device *device);

private:

    uint16_t _address;

    std::vector<Device*> _devices;
};

class Device
{

public:

    virtual void attach(Bus &bus);

    virtual void detach();

    virtual bool contains(uint16_t addr) const = 0;

    virtual void read(uint16_t address, uint8_t &buffer) const = 0;

    virtual void write(uint16_t address, uint8_t data) = 0;

protected:

    friend class Bus;

    virtual void read(uint8_t &buffer) const;

    virtual void write(uint8_t data);

    Bus *_bus;
};

/* Devices of CPU */

/**
 * @brief RAM
 * 
 * Main memory of CPU
 */
class RandomAccessMemory: public Device
{

public:

    static const int RamSize = 0x0800; // 2KB
    static const int RamMask = 0x07ff;
    static const int RamLowerBound = 0x0000;
    static const int RamUpperBound = 0x2000;

    RandomAccessMemory();

    bool contains(uint16_t addr) const override;

    void read(uint16_t address, uint8_t &buffer) const override;

    void write(uint16_t address, uint8_t data) override;

private:

    std::array<uint8_t, RamSize> _memory;
};

/**
 * @brief ROM
 * 
 * PRG-ROM of CPU
 */
class ReadOnlyMemory: public Device
{

public:

    static const int RomMask = 0x7fff;
    static const int RomBankCount = 2; // 4KB * 2
    static const int RomLowerBankBase = 0x8000;
    static const int RomUpperBankBase = 0xC000;

    ReadOnlyMemory(const std::vector<uint8_t> &memory);

    bool contains(uint16_t addr) const override;

    void read(uint16_t address, uint8_t &buffer) const override;

    void write(uint16_t address, uint8_t data) override;

private:

    uint16_t _base;

    const std::vector<uint8_t> &_memory;
};

/* For PPU MMIO Registers */
const int PpuMmioLowerBound  = 0x2000;
const int PpuMmioUpperBound  = 0x4000;
const int PpuMmioAddressMask = 0x07;   // 0000 0111

/* Devices of PPU */

/**
 * @brief VRAM
 * 
 * Memory of PPU, or so called Name Tables
 */
class VideoRandomAccessMemory: public Device
{

public:

    static const int VramSize = 0x0800; // 2KB
    static const int VramMask = 0x07ff;
    static const int VramLowerBound = 0x2000;
    static const int VramUpperBound = 0x3F00;

    VideoRandomAccessMemory();

    bool contains(uint16_t addr) const override;

    void read(uint16_t address, uint8_t &buffer) const override;

    void write(uint16_t address, uint8_t data) override;

private:

    std::array<uint8_t, VramSize> _memory;
};

/**
 * @brief PPU Pattern Tables
 * 
 * Mapped by a CHR-ROM or CHR-RAM of a cartridge
 */
class PatternTables: public Device
{

public:

    static const int TableSize  = 0x1000;
    static const int TotalSize  = 0x2000;
    static const int TableCount = 2; // 4KB * 2
    static const int TableLowerBankBase = 0x0000;
    static const int TableUpperBankBase = 0x1000;

    PatternTables(const std::vector<uint8_t> &memory);

    bool contains(uint16_t addr) const override;

    void read(uint16_t address, uint8_t &buffer) const override;

    void write(uint16_t address, uint8_t data) override;

private:

    uint16_t _base;

    const std::vector<uint8_t> &_memory;
};

/* Memory Mapper */

/**
 * @brief MMC
 */
class MemoryManagementController
{
    /* TODO */
};

} // namespace tones

#endif // _TONES_DEVICE_H_