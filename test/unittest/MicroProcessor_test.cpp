
#include <memory>

#include <gtest/gtest.h>

#include "Clock.h"
#include "Device.h"
#include "Cartridge.h"
#include "MicroProcessor.h"

#include "roms.h"
#include "CpuRegParser.hpp"

using namespace tones;

class DebuggingCPU : public MicroProcessor
{
public:

    MicroProcessor::Registers_t regs;

    DebuggingCPU(Bus &bus) : MicroProcessor(bus) {}

    void step()
    {
        _tick();
        dump(regs);
    }
};

class MicroProcessorTest : public ::testing::TestWithParam<std::string>
{

protected:

    MicroProcessorTest() : _cpu(_mbus) {}

    void SetUp() override
    {
        _pram.attach(_mbus);
    }

    Bus _mbus, _vbus;
    RandomAccessMemory _pram;
    DebuggingCPU _cpu;

    CpuRegParser _parser;
};

TEST_F(MicroProcessorTest, Reset)
{
    std::string filepath = getRomBin(ROM_LOAD);
    auto card = CartridgeFactory::createCartridge(filepath);
    ASSERT_NE(card, nullptr);

    card->attach(_mbus, _vbus);
    _cpu.reset();
    _cpu.dump(_cpu.regs);

    EXPECT_EQ(_cpu.regs.A, 0);
    EXPECT_EQ(_cpu.regs.X, 0);
    EXPECT_EQ(_cpu.regs.Y, 0);
    EXPECT_EQ(_cpu.regs.S, ::cpu::DefaultStack);
    EXPECT_EQ(_cpu.regs.P, ::cpu::DefaultStatus);

    int pc = ReadOnlyMemory::RomLowerBankBase;
    EXPECT_EQ(_cpu.regs.PC, pc);
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

        auto *regs = _parser.next();
        ASSERT_NE(regs, nullptr);

        ASSERT_EQ(_cpu.regs.A, regs->A) << *(_parser.code());
        ASSERT_EQ(_cpu.regs.X, regs->X) << *(_parser.code());
        ASSERT_EQ(_cpu.regs.Y, regs->Y) << *(_parser.code());
        ASSERT_EQ(_cpu.regs.S, regs->S) << *(_parser.code());
        ASSERT_EQ(_cpu.regs.P, regs->P) << *(_parser.code());
        ASSERT_EQ(_cpu.regs.PC, regs->PC) << *(_parser.code());
    }
}

INSTANTIATE_TEST_SUITE_P(BasicTest,
                         MicroProcessorTest,
                         testing::Values(
                            ROM_NOP,
                            ROM_LOAD,
                            ROM_STATUS,
                            ROM_TRANSFER,
                            ROM_ARITHMETIC,
                            ROM_LOGIC
                        ));

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
