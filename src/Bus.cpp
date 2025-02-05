
#include "Bus.h"

namespace tones {

/* Accessible */

void Accessible::attach(Bus &bus)
{
    _bus = &bus;
    _bus->attach(this);
}

void Accessible::detach()
{
    _bus->detach(this);
    _bus = nullptr;
}

void Accessible::read(uint8_t &buffer) const
{
    if (contains(_bus->address()))
        read(_bus->address(), buffer);
}

void Accessible::write(uint8_t data)
{
    if (contains(_bus->address()))
        write(_bus->address(), data);
}

/* Bus */

uint16_t Bus::address() const
{
    return _address;
}

void Bus::read(uint16_t address, uint8_t &buffer)
{
    _address = address;
    for (auto it = _devices.begin(); it != _devices.end(); ++it)
        (*it)->read(buffer);
}

void Bus::write(uint16_t address, uint8_t data)
{
    _address = address;
    for (auto it = _devices.begin(); it != _devices.end(); ++it)
        (*it)->write(data);
}

void Bus::attach(Accessible *device)
{
    _devices.push_back(device);
}

void Bus::detach(Accessible *device)
{
    for (auto it = _devices.begin(); it != _devices.end(); ++it) {
        if (*it == device) {
            _devices.erase(it);
            break;
        }
    }
}

} // namespace tones
