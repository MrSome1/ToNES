#ifndef _TONES_DATABUS_H_
#define _TONES_DATABUS_H_

#include <cinttypes>
#include <vector>

namespace tones {

class DataBus;

class Device
{

public:

    virtual void attach(DataBus &bus);

    virtual void read(uint8_t &buffer);

    virtual void write(uint8_t data);

    virtual bool contains(uint16_t addr) = 0;

    virtual void read(uint16_t address, uint8_t &buffer) = 0;

    virtual void write(uint16_t address, uint8_t data) = 0;

protected:

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