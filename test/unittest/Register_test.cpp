
#include <gtest/gtest.h>

#include "Register.h"

using namespace tones::reg;

TEST(BaseRegisterTest, Shift)
{
    ::Base<uint8_t> reg;
    reg.value = 0x55;

    EXPECT_EQ(reg << 1,  0xaa);
    EXPECT_EQ(reg.value, 0x55);

    EXPECT_EQ(reg >> 1,  0x2a);
    EXPECT_EQ(reg.value, 0x55);
}

TEST(CycleRegisterTest, Increment)
{
    ::Cycle reg;
    reg.reset(32);

    for (int i = 1; i < 32; ++i) {
        ++reg;
    }
    EXPECT_TRUE(reg.full());
    EXPECT_TRUE(reg.overflow());
    EXPECT_EQ(reg.value, 31);

    ++reg;
    EXPECT_FALSE(reg.full());
    EXPECT_FALSE(reg.overflow());
    EXPECT_EQ(reg.value, 0);
}

TEST(CycleRegisterTest, Decrement)
{
    ::Cycle reg;
    reg.reset(32);

    EXPECT_TRUE(reg.empty());

    --reg;
    EXPECT_EQ(reg.value, 31);

    for (int i = 1; i < 32; ++i) {
        --reg;
    }
    EXPECT_EQ(reg.value, 0);
}