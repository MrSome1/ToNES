#ifndef _TONES_DATABUS_H_
#define _TONES_DATABUS_H_

#include <cinttypes>
#include <vector>

namespace tones {

class DataBus;

class Device
{

public:
    Device();
    virtual ~Device(); 

    virtual void attach(DataBus &bus);

    virtual bool contains(uint16_t addr) = 0;

    virtual uint8_t read(uint16_t addr) = 0;

    virtual void write(uint16_t addr, uint8_t data) = 0;

protected:
    DataBus *_bus;
};

class DataBus
{

public:
    DataBus();
    ~DataBus();

    void attach(Device &device);

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

protected:
    Device* searchDevice(uint16_t addr);

private:
    std::vector<Device*> _devices;
};

} // namespace tones

#endif // _TONES_DATABUS_H_ 