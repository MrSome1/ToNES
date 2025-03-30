
#include <gtest/gtest.h>

#include "Clock.h"
#include "Device.h"
#include "Cartridge.h"
#include "MicroProcessor.h"

#include "roms.h"
#include "RomParser.hpp"
#include "LogParser.hpp"

using namespace tones;

class MicroProcessorTest : public ::testing::TestWithParam<std::string>
{
public:

    static const int StatusMask = 0b11001111;

public:

    MicroProcessorTest() : _cpu(_mbus) {}

    void load_rom(const std::string &rom)
    {
        _card = CartridgeFactory::createCartridge(getRomBin(rom));
        ASSERT_NE(_card, nullptr);

        _card->attach(_mbus, _vbus);
        _cpu.reset();
    }

    void compare_step_by_step(LogParser *parser, int steps=0)
    {
        steps = !steps ? parser->size() : steps;
        ASSERT_NE(steps, 0);

        for (int i = 0; i < steps; ++i) {
            std::string info = hint(i, parser->line());

            _cpu.dump(_regs);

            ASSERT_EQ(_regs.PC, parser->regs().PC) << info;
            ASSERT_EQ(_regs.A,  parser->regs().A)  << info;
            ASSERT_EQ(_regs.X,  parser->regs().X)  << info;
            ASSERT_EQ(_regs.Y,  parser->regs().Y)  << info;
            ASSERT_EQ(_regs.S,  parser->regs().S)  << info;

            // We don't care about the break bit and the undefined bit
            ASSERT_EQ(_regs.P & StatusMask,  parser->regs().P & StatusMask) << info;

            _cpu.step();
            parser->next();
        }
    }

    std::string hint(int num, const std::string &line)
    {
        return "[Line " + std::to_string(++num) + "] " + line;
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

    CartridgePtr _card;
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

    FceuxLogParser parser;
    parser.load(getRomLog(rom));

    load_rom(rom);
    compare_step_by_step(&parser);
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

TEST_F(MicroProcessorTest, NesTest)
{
    std::string rom = "nestest";

    NintendulatorLogParser parser;
    parser.load(getRomLog(rom));

    load_rom(rom);
    _cpu.jump(0xc000); // jump to the main test function

    // TODO: Use tick()
    compare_step_by_step(&parser, 5004); // unofficial instructions after 5004
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
