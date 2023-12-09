
#include "DataBus.h"

namespace tones {

/* Device */

void Device::attach(DataBus &bus)
{
    _bus = &bus;
}

void Device::read(uint8_t &buffer)
{
    if (contains(_bus->address()))
        read(_bus->address(), buffer);
}

void Device::write(uint8_t data)
{
    if (contains(_bus->address()))
        write(_bus->address(), data);
}

/* DataBus */

DataBus::DataBus()
{

}

DataBus::~DataBus()
{

}

void DataBus::attach(Device &device)
{
    _devices.push_back(&device);
}

uint16_t DataBus::address() const
{
    return _address;
}

void DataBus::read(uint16_t address, uint8_t &buffer)
{
    _address = address;
    for (auto it = _devices.begin(); it != _devices.end(); ++it)
        (*it)->read(buffer);
}

void DataBus::write(uint16_t address, uint8_t data)
{
    _address = address;
    for (auto it = _devices.begin(); it != _devices.end(); ++it)
        (*it)->write(data);
}

} // namespace tones