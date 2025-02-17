
#include <gtest/gtest.h>

#include "Clock.h"
#include "Device.h"
#include "Cartridge.h"
#include "MicroProcessor.h"

#include "roms.h"
#include "RomParser.hpp"

using namespace tones;

class MicroProcessorTest : public ::testing::TestWithParam<std::string>
{

public:

    MicroProcessorTest() : _cpu(_mbus) {}

    const std::string hint(const RomParser::Line &line)
    {
        return "Line " + std::to_string(line.num) + ": " + line.code;
    }

protected:

    void SetUp() override
    {
        _pram.attach(_mbus);
    }

    Bus _mbus, _vbus;
    RandomAccessMemory _pram;

    MicroProcessor _cpu;
    MicroProcessor::Registers_t _regs;

    RomParser _parser;
};

TEST_F(MicroProcessorTest, Reset)
{
    std::string filepath = getRomBin(ROM_LOAD);
    auto card = CartridgeFactory::createCartridge(filepath);
    ASSERT_NE(card, nullptr);

    card->attach(_mbus, _vbus);
    _cpu.reset();
    _cpu.dump(_regs);

    EXPECT_EQ(_regs.A, 0);
    EXPECT_EQ(_regs.X, 0);
    EXPECT_EQ(_regs.Y, 0);
    EXPECT_EQ(_regs.S, ::cpu::DefaultStack);
    EXPECT_EQ(_regs.P, ::cpu::DefaultStatus);

    int pc = ReadOnlyMemory::RomLowerBankBase;
    EXPECT_EQ(_regs.PC, pc);
}

TEST_F(MicroProcessorTest, NMI)
{
    std::string filepath = getRomBin(ROM_INTERRUPT);
    auto card = CartridgeFactory::createCartridge(filepath);
    ASSERT_NE(card, nullptr);

    card->attach(_mbus, _vbus);
    _cpu.reset();

    _cpu.nmi();
    _cpu.dump(_regs);

    EXPECT_EQ(_regs.S, 0xfa);
    EXPECT_EQ(_regs.PC, 0x8002);
}

TEST_P(MicroProcessorTest, Instructions)
{
    std::string rom = GetParam();

    std::string romSrc = getRomSrc(rom);
    _parser.load(romSrc);
    ASSERT_NE(_parser.size(), 0);

    std::string romBin = getRomBin(rom);
    auto card = CartridgeFactory::createCartridge(romBin);
    ASSERT_NE(card, nullptr);

    card->attach(_mbus, _vbus);
    _cpu.reset();

    for (int i = 0; i < _parser.size(); ++i) {
        _cpu.step();
        _cpu.dump(_regs);

        auto line = _parser.line();

        if (RomParser::hasRam(line)) {
            uint8_t val;
            _pram.read(line.ramAddr, val);
            EXPECT_EQ(val, line.ramValue) << hint(line);
        }

        ASSERT_EQ(_regs.A, line.regs.A)   << hint(line);
        ASSERT_EQ(_regs.X, line.regs.X)   << hint(line);
        ASSERT_EQ(_regs.Y, line.regs.Y)   << hint(line);
        ASSERT_EQ(_regs.S, line.regs.S)   << hint(line);
        ASSERT_EQ(_regs.P, line.regs.P)   << hint(line);
        ASSERT_EQ(_regs.PC, line.regs.PC) << hint(line);

        _parser.next();
    }
}

INSTANTIATE_TEST_SUITE_P(BasicTest,
                         MicroProcessorTest,
                         testing::Values(
                            ROM_LOAD,
                            ROM_STACK,
                            ROM_STATUS,
                            ROM_TRANSFER,
                            ROM_BRANCH,
                            ROM_JUMP,
                            ROM_LOGIC,
                            ROM_ARITHMETIC,
                            ROM_INTERRUPT
                        ));

INSTANTIATE_TEST_SUITE_P(AddressingModeTest,
                         MicroProcessorTest,
                         testing::Values(
                            ROM_ZP,
                            ROM_ZPX,
                            ROM_ZPY,
                            ROM_ABS,
                            ROM_ABX,
                            ROM_ABY,
                            ROM_IDX,
                            ROM_IDY,
                            ROM_IND
                        ));

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
