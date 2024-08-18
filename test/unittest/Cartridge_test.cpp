
#include <gtest/gtest.h>

#include "Device.h"
#include "Cartridge.h"
#include "MicroProcessor.h"

#include "roms.h"

using namespace tones;

class CartridgeTest : public ::testing::TestWithParam<std::string>
{
public:
    uint16_t genRomAddr(uint16_t index)
    {
        return ReadOnlyMemory::RomLowerBankBase + index;
    }

protected:
    Bus _mbus;
    Bus _vbus;
};

TEST_P(CartridgeTest, Invalid)
{
    std::string filepath = GetParam();
    auto card = CartridgeFactory::createCartridge(filepath);
    ASSERT_EQ(card, nullptr);
}

INSTANTIATE_TEST_SUITE_P(InvalidPathTest,
                         CartridgeTest,
                         testing::Values(
                            "",
                            ROM_ROOT_DIR,
                            ROM_LOAD + std::string(ROM_BIN_EXT),
                            getRomSrc(ROM_LOAD)
                         ));

TEST_F(CartridgeTest, iNES)
{
    uint8_t buffer;
    std::string filepath = getRomBin(ROM_LOAD);

    auto card = CartridgeFactory::createCartridge(filepath);
    ASSERT_NE(card, nullptr);

    card->attach(_mbus, _vbus);

    // LDA #$1
    _mbus.read(genRomAddr(0), buffer);
    EXPECT_EQ(buffer, 0xa9);

    _mbus.read(genRomAddr(1), buffer);
    EXPECT_EQ(buffer, 0x01);

    // LDX #$1
    _mbus.read(genRomAddr(2), buffer);
    EXPECT_EQ(buffer, 0xa2);

    _mbus.read(genRomAddr(3), buffer);
    EXPECT_EQ(buffer, 0x01);

    // LDY #$1
    _mbus.read(genRomAddr(4), buffer);
    EXPECT_EQ(buffer, 0xa0);

    _mbus.read(genRomAddr(5), buffer);
    EXPECT_EQ(buffer, 0x01);

    // ResetVector
    _mbus.read(cpu::ResetVector, buffer);
    EXPECT_EQ(buffer, ReadOnlyMemory::RomLowerBankBase & 0xff);

    _mbus.read(cpu::ResetVector + 1, buffer);
    EXPECT_EQ(buffer, ReadOnlyMemory::RomLowerBankBase >> 8);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
