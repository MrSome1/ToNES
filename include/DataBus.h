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

class DataBus;

class Device
{

public:

    Device(uint16_t base, uint16_t size);

    virtual void attach(DataBus &bus);

    virtual void read(uint8_t &buffer) const;

    virtual void write(uint8_t data);

    virtual bool contains(uint16_t addr) const;

    virtual void read(uint16_t address, uint8_t &buffer) const = 0;

    virtual void write(uint16_t address, uint8_t data) = 0;

protected:

    uint16_t _base;
    uint16_t _size;
    DataBus *_bus;
};

class DataBus
{

public:

    DataBus();
    ~DataBus();

    void attach(Device &device);

    uint16_t address() const;

    void read(uint16_t address, uint8_t &buffer);

    void write(uint16_t address, uint8_t data);

private:

    uint16_t _address;

    std::vector<Device*> _devices;
};

} // namespace tones

#endif // _TONES_DATABUS_H_ 