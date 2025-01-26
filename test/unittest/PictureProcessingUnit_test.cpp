
#include <gtest/gtest.h>

#include "Clock.h"
#include "Device.h"
#include "Register.h"
#include "PictureProcessingUnit.h"
#include "MotherBoard.h"

using namespace tones;

class PictureProcessingUnitTest : public ::testing::Test
{

public:

    PictureProcessingUnitTest()
        : _count(0)
        , _ppu(_vbus, _mbus)
        , _odma(OAMDMA, ppu::OAMDATA, ppu::SpriteMemorySize) {}

    // Dummy non-maskable interruption handler
    void nmi()
    {
        ++_count;
    }

protected:

    void SetUp() override
    {
        _pram.attach(_mbus);
        _vram.attach(_vbus);

        _odma.attach(_mbus);

        _ppu.reset();
        _ppu.setBlankHandler([&] () { nmi(); });
    }

    int _count;

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
    for (uint16_t addr = 0x0100; addr < 0x0200; ++addr) {
        _mbus.write(addr, buff++);
    }

    _mbus.write(ppu::OAMADDR, 0x00);
    _mbus.write(OAMDMA, 0x01);

    for (uint8_t addr = 0; addr < ppu::SpriteMemorySize - 1; ++addr) {
        _mbus.write(ppu::OAMADDR, addr);
        _mbus.read(ppu::OAMDATA, buff);
        ASSERT_EQ(addr, buff);
    }
}

TEST_F(PictureProcessingUnitTest, PpuDataReadWrite)
{
    uint8_t addr;
    uint8_t buff;

    // Init base address to write
    reg::getMSB(VideoRandomAccessMemory::VramLowerBound, addr);
    _mbus.write(ppu::PPUADDR, addr);

    reg::getLSB(VideoRandomAccessMemory::VramLowerBound, addr);
    _mbus.write(ppu::PPUADDR, addr);

    for (int i = 0; i < VideoRandomAccessMemory::VramSize; ++i) {
        buff = i & 0xff;
        _mbus.write(ppu::PPUDATA, buff);
    }

    // Init base address to read
    reg::getMSB(VideoRandomAccessMemory::VramLowerBound, addr);
    _mbus.write(ppu::PPUADDR, addr);

    reg::getLSB(VideoRandomAccessMemory::VramLowerBound, addr);
    _mbus.write(ppu::PPUADDR, addr);

    for (int i = 0; i < VideoRandomAccessMemory::VramSize; ++i) {
        _mbus.read(ppu::PPUDATA, buff);
        ASSERT_EQ(buff, i & 0xff);

        _vbus.read(VideoRandomAccessMemory::VramLowerBound + i, buff);
        ASSERT_EQ(buff, i & 0xff);
    }
}

// TODO: Write PPUCTRL

// ControllerBit::XY
TEST_F(PictureProcessingUnitTest, ControllNametableBase)
{
    // TODO
}

// ControllerBit::I
TEST_F(PictureProcessingUnitTest, ControllIncrement)
{
    const int size = VideoRandomAccessMemory::VramSize >> 5;
    const int step = 0x20; // 32

    uint8_t addr;
    uint8_t buff;

    _mbus.write(ppu::PPUCTRL, 0x04);

    // Init base address to write
    reg::getMSB(VideoRandomAccessMemory::VramLowerBound, addr);
    _mbus.write(ppu::PPUADDR, addr);

    reg::getLSB(VideoRandomAccessMemory::VramLowerBound, addr);
    _mbus.write(ppu::PPUADDR, addr);

    for (int i = 0; i < size; i += step) {
        buff = i & 0xff;
        _mbus.write(ppu::PPUDATA, buff);

        _vbus.read(VideoRandomAccessMemory::VramLowerBound + i, buff);
        ASSERT_EQ(buff, i & 0xff);
    }
}

// ControllerBit::S
TEST_F(PictureProcessingUnitTest, ControllSpriteBase)
{
    // TODO
}

// ControllerBit::B
TEST_F(PictureProcessingUnitTest, ControllBackgroundBase)
{
    // TODO
}

// ControllerBit::H
TEST_F(PictureProcessingUnitTest, ControllSpriteSize)
{
    // TODO
}

// ControllerBit::V
TEST_F(PictureProcessingUnitTest, ControllNmiEnable)
{
    const int size = (ppu::NTSC.lineEnd + 1) * (ppu::NTSC.dotEnd + 1) / 3;

    // NMI disabled
    for (int i = 0; i < size; ++i) {
        _ppu.tick();
    }

    EXPECT_EQ(_count, 0);

    // NMI enabled
    _mbus.write(ppu::PPUCTRL, 0x80);

    for (int i = 0; i < size; ++i) {
        _ppu.tick();
    }

    EXPECT_EQ(_count, 1);
}

// TODO: Write PPUMASK

// TODO: Write PPUSCROLL

// TODO: Read PPUSTATUS
TEST_F(PictureProcessingUnitTest, ReadStatus)
{
    const int size = (ppu::NTSC.lineEnd) * (ppu::NTSC.dotEnd + 1) / 3;

    for (int i = 0; i < size; ++i) {
        _ppu.tick();
    }

    uint8_t buff;
    _mbus.read(ppu::PPUSTATUS, buff);
    EXPECT_TRUE(buff & 0x80) << "PPUSTATUS: " << (int)buff;
    _mbus.read(ppu::PPUSTATUS, buff);
    EXPECT_FALSE(buff & 0x80) << "PPUSTATUS: " << (int)buff;
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
