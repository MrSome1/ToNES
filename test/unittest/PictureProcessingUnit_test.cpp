
#include <gtest/gtest.h>

#include "Clock.h"
#include "Device.h"
#include "PictureProcessingUnit.h"
#include "MotherBoard.h"

using namespace tones;

class PictureProcessingUnitTest : public ::testing::Test
{

public:

    PictureProcessingUnitTest()
        : _ppu(_vbus, _mbus)
        , _odma(OAMDMA, ppu::OAMDATA, ppu::SpriteMemorySize) {}

    // Dummy non-maskable interruption handler
    void nmi()
    {
        // TODO
    }

protected:

    void SetUp() override
    {
        _pram.attach(_mbus);
        _vram.attach(_vbus);

        _odma.attach(_mbus);

        _ppu.setBlankHandler([&] () { nmi(); });
    }

    Bus _mbus, _vbus;

    RandomAccessMemory _pram;
    VideoRandomAccessMemory _vram;

    PictureProcessingUnit _ppu;
    DirectMemoryAccess _odma;
};

TEST_F(PictureProcessingUnitTest, OamReadWrite)
{
    uint8_t buff;

    _mbus.write(ppu::OAMADDR, 0);
    for (buff = 0; buff < ppu::SpriteMemorySize - 1; ++buff) {
        _mbus.write(ppu::OAMDATA, buff);
    }

    for (uint8_t addr = 0; addr < ppu::SpriteMemorySize - 1; ++addr) {
        _mbus.write(ppu::OAMADDR, addr);
        _mbus.read(ppu::OAMDATA, buff);
        ASSERT_EQ(addr, buff);
    }
}

TEST_F(PictureProcessingUnitTest, OamDma)
{
    uint8_t buff = 0;

    // Initialize page 1
    for (uint16_t addr = 256; addr < 512; ++addr) {
        _mbus.write(addr, buff++);
    }

    _mbus.write(ppu::OAMADDR, 0);
    _mbus.write(OAMDMA, 1);

    for (uint8_t addr = 0; addr < ppu::SpriteMemorySize - 1; ++addr) {
        _mbus.write(ppu::OAMADDR, addr);
        _mbus.read(ppu::OAMDATA, buff);
        ASSERT_EQ(addr, buff);
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
