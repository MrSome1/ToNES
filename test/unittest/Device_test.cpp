
#include <stdlib.h>
#include <vector>

#include <gtest/gtest.h>

#include "Device.h"

using namespace tones;

const int RomSize = ReadOnlyMemory::RomUpperBankBase - ReadOnlyMemory::RomLowerBankBase;

class DeviceTest : public ::testing::Test
{

protected:

    DeviceTest() : _data(RomSize, 0), _rom(_data) {}

    void SetUp() override
    {
        _ram.attach(_bus);
        _rom.attach(_bus);
    }

    void TearDown() override
    {
        _ram.detach();
        _rom.detach();
    }

    Bus _bus;
    RandomAccessMemory _ram;
    ReadOnlyMemory _rom;
    std::vector<uint8_t> _data;
};

TEST_F(DeviceTest, RamReadWrite)
{
    uint16_t addr;
    uint8_t buffer, data;

    // Sequencely
    for (uint16_t i = 0; i < RandomAccessMemory::RamSize; ++i) {
        data = rand() & 0xff;
        _bus.write(i, data);
        _bus.read(i, buffer);
        EXPECT_EQ(buffer, data);
    }

    // Randomly
    for (uint16_t i = 0; i < RandomAccessMemory::RamSize; ++i) {
        data = i & 0xff;
        addr = rand() & RandomAccessMemory::RamMask;
        _bus.write(addr, data);
        _bus.read(addr, buffer);
        EXPECT_EQ(buffer, data);
    }
}

TEST_F(DeviceTest, RamMirror)
{
    uint16_t addr;
    uint8_t buffer, data;

    EXPECT_EQ(true, _ram.contains(RandomAccessMemory::RamLowerBound));
    EXPECT_EQ(true, _ram.contains(RandomAccessMemory::RamUpperBound - 1));
    EXPECT_EQ(false, _ram.contains(RandomAccessMemory::RamUpperBound));

    // Sequencely
    for (uint16_t i = RandomAccessMemory::RamSize; i < RandomAccessMemory::RamUpperBound; ++i) {
        data = rand() & 0xff;
        _bus.write(i, data);

        addr = i & RandomAccessMemory::RamMask;

        _bus.read(addr, buffer);
        EXPECT_EQ(buffer, data);

        _bus.read(addr + RandomAccessMemory::RamSize, buffer);
        EXPECT_EQ(buffer, data);

        _bus.read(addr + 2 * RandomAccessMemory::RamSize, buffer);
        EXPECT_EQ(buffer, data);

        _bus.read(addr + 3 * RandomAccessMemory::RamSize, buffer);
        EXPECT_EQ(buffer, data);
    }

    // Randomly
    for (uint16_t i = RandomAccessMemory::RamSize; i < RandomAccessMemory::RamUpperBound; ++i) {
        data = i & 0xff;
        addr = rand() % RandomAccessMemory::RamUpperBound;
        _bus.write(addr, data);

        addr &= RandomAccessMemory::RamMask;

        _bus.read(addr, buffer);
        EXPECT_EQ(buffer, data);

        _bus.read(addr + RandomAccessMemory::RamSize, buffer);
        EXPECT_EQ(buffer, data);

        _bus.read(addr + 2 * RandomAccessMemory::RamSize, buffer);
        EXPECT_EQ(buffer, data);

        _bus.read(addr + 3 * RandomAccessMemory::RamSize, buffer);
        EXPECT_EQ(buffer, data);
    }
}

TEST_F(DeviceTest, RomReadWrite)
{
    uint16_t addr;
    uint8_t buffer, data;

    for (int i = 0; i < RomSize; ++i)
        _data[i] = i & 0xff;
    
    // Sequencely
    for (uint16_t i = 0; i < RomSize; ++i) {
        addr = (ReadOnlyMemory::RomLowerBankBase + i) & 0xffff;

        data = i & 0xff;
        _bus.write(addr, data + 1);

        _bus.read(addr, buffer);
        EXPECT_EQ(buffer, data);
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
