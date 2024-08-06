
#include <stdlib.h>
#include <vector>

#include <gtest/gtest.h>

#include "Clock.h"
#include "Device.h"
#include "MicroProcessor.h"

using namespace tones;

class MicroProcessorTest : public ::testing::Test
{

protected:

    MicroProcessorTest() : _cpu(_bus), _data(1024, 0), _rom(_data) {}

    void SetUp() override
    {
        _ram.attach(_bus);
        _rom.attach(_bus);

        _cpu.attach(_clock);
    }

    void TearDown() override
    {
        _ram.detach();
        _rom.detach();
    }

    Bus _bus;
    Clock _clock;
    RandomAccessMemory _ram;
    MicroProcessor _cpu;

    std::vector<uint8_t> _data;
    ReadOnlyMemory _rom;
};

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
