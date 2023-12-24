/* CPU Memory Map
 *
 * ------------------------------------- $10000
 *    Cartridge   | PRG-ROM Upper Bank
 *                +--------------------- $C000
 *     PRG-ROM    | PRG-ROM Lower Bank
 * ---------------+--------------------- $8000
 *      SRAM      | Cartridge Save RAM
 * ---------------+--------------------- $6000
 *  Expansion ROM |
 * ---------------+--------------------- $4020
 *                | I/O Registers
 *                +--------------------- $4000
 *  I/O Registers | Mirrors $2000-$2007
 *                +--------------------- $2008
 *                | I/O Registers
 * ---------------+--------------------- $2000
 *                | Mirrors $0000-$07FF
 *                +--------------------- $0800
 *                | RAM
 *       RAM      +--------------------- $0200
 *                | Stack
 *                +--------------------- $0100
 *                | Zero Page
 * ------------------------------------- $0000
 */

#ifndef _TONES_DATABUS_H_
#define _TONES_DATABUS_H_

#include <cinttypes>
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

/**
 * @brief RAM
 */
class RandomAccessMemory: public Device
{
    static const uint16_t RamSize = 0x2000;
    static const uint16_t RamMask = 0x07ff; // for mirroring 4 times

public:

    RandomAccessMemory();

    bool contains(uint16_t addr) const override;

    void read(uint16_t address, uint8_t &buffer) const override;

    void write(uint16_t address, uint8_t data) override;

private:

    std::vector<uint8_t> _memory;
};

/**
 * @brief ROM
 */
class ReadOnlyMemory: public Device
{
    static const uint16_t RomLowerBankBase = 0x8000;
    static const uint16_t RomUpperBankBase = 0xC000;

public:

    ReadOnlyMemory(const std::vector<uint8_t> &memory);

    bool contains(uint16_t addr) const override;

    void read(uint16_t address, uint8_t &buffer) const override;

    void write(uint16_t address, uint8_t data) override;

private:

    uint16_t _base;

    const std::vector<uint8_t> &_memory;
};

/**
 * @brief MMC
 */
class MemoryManagementController
{
    /* TODO */
};

} // namespace tones

#endif // _TONES_DATABUS_H_ 