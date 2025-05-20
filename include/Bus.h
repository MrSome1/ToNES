
#ifndef _TONES_BUS_H_
#define _TONES_BUS_H_

#include <cinttypes>
#include <vector>

namespace tones {

class Bus;

const int AddressSpace = 0x10000;

/**
 * @brief Abstraction of an device
 * 
 * A device which can be read from and written to, with
 * the system bus
 */
class Accessible
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
 * @brief Abstraction of the system bus
 * 
 * This class inclues the Address Bus, Control Bus and
 * Data Bus, actually
 */
class Bus
{

public:

    //! Abstraction of the Address Bus
    uint16_t address() const;

    //! Abstraction of the Data Bus, with Control Bus of read mode
    void read(uint16_t address, uint8_t &buffer);

    //! Abstraction of the Data Bus, with Control Bus of write mode
    void write(uint16_t address, uint8_t data);

protected:

    friend class Accessible;

    //! Mount a device
    void attach(Accessible *device);

    //! Unmount a device
    void detach(Accessible *device);

private:

    uint16_t _address;

    std::vector<Accessible*> _devices;
};

} // namespace tones

#endif // _TONES_BUS_H_
