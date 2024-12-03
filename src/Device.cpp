
#include "Device.h"

namespace tones {

/* Bus */

Bus::Bus() {}

Bus::~Bus() {}

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

void Bus::attach(Device *device)
{
    _devices.push_back(device);
}

void Bus::detach(Device *device)
{
    for (auto it = _devices.begin(); it != _devices.end(); ++it) {
        if (*it == device) {
            _devices.erase(it);
            break;
        }
    }
}

/* Device */

void Device::attach(Bus &bus)
{
    _bus = &bus;
    _bus->attach(this);
}

void Device::detach()
{
    _bus->detach(this);
    _bus = nullptr;
}

void Device::read(uint8_t &buffer) const
{
    if (contains(_bus->address()))
        read(_bus->address(), buffer);
}

void Device::write(uint8_t data)
{
    if (contains(_bus->address()))
        write(_bus->address(), data);
}

/* RandomAccessMemory */

RandomAccessMemory::RandomAccessMemory() {}

bool RandomAccessMemory::contains(uint16_t addr) const
{
    return addr < RamUpperBound;
}

void RandomAccessMemory::read(uint16_t address, uint8_t &buffer) const
{
    buffer = _memory[address & RamMask];
}

void RandomAccessMemory::write(uint16_t address, uint8_t data)
{
    _memory[address & RamMask] = data;
}

/* ReadOnlyMemory */

ReadOnlyMemory::ReadOnlyMemory(const std::vector<uint8_t> &memory)
    : _memory(memory) {}

bool ReadOnlyMemory::contains(uint16_t addr) const
{
    return addr >= RomLowerBankBase;
}

void ReadOnlyMemory::read(uint16_t address, uint8_t &buffer) const
{
    buffer = _memory[address & RomMask];
}

void ReadOnlyMemory::write(uint16_t address, uint8_t data)
{
    /* Just do nothing at all*/
}

/* VideoRandomAccessMemory */

VideoRandomAccessMemory::VideoRandomAccessMemory() {}

bool VideoRandomAccessMemory::contains(uint16_t addr) const
{
    return addr >= VramLowerBound && addr < VramUpperBound;
}

void VideoRandomAccessMemory::read(uint16_t address, uint8_t &buffer) const
{
    buffer = _memory[address & VramMask];
}

void VideoRandomAccessMemory::write(uint16_t address, uint8_t data)
{
    _memory[address & VramMask] = data;
}

/* PatternTalbe */

PatternTables::PatternTables(const std::vector<uint8_t> &memory)
    : _memory(memory) {}

bool PatternTables::contains(uint16_t addr) const
{
    return addr < TotalSize;
}

void PatternTables::read(uint16_t address, uint8_t &buffer) const
{
    buffer = _memory[address];
}

void PatternTables::write(uint16_t address, uint8_t data)
{
    /* TODO */
}

} // namespace tones