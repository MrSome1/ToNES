
#include "DataBus.h"

namespace tones {

Device::Device()
{

}

Device::~Device()
{

}

void Device::attach(DataBus &bus)
{
    _bus = &bus;
}

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

uint8_t DataBus::read(uint16_t addr)
{
    Device *device = searchDevice(addr);
    return device == nullptr ? 0 : device->read(addr);
}

void DataBus::write(uint16_t addr, uint8_t data)
{
    Device *device = searchDevice(addr);
    if (device != nullptr)
        device->write(addr, data);
}

Device* DataBus::searchDevice(uint16_t addr)
{
    for (auto it = _devices.begin(); it != _devices.end(); ++it) {
        if ((*it)->contains(addr))
            return *it;
    }
    return nullptr; // should never be here
}

} // namespace tones