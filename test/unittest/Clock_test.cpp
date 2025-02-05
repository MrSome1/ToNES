
#include <gtest/gtest.h>

#include "Clock.h"

using namespace tones;

class Counter : public Tickable
{

public:

    void tick() override { ++value; }

    int operator()() { return value; }

protected:

    int value = 0;
};

TEST(ClockTest, Tick)
{
    const int num = 10;
    const int mul = 5;

    Clock clock;
    Counter counter_1;
    Counter counter_2;

    counter_1.attach(clock, 1);
    counter_2.attach(clock, mul);

    for (int i = 1; i <= num; ++i) {
        clock.tick();
        EXPECT_EQ(counter_1(), i);
        EXPECT_EQ(counter_2(), mul * i);
    }
}

TEST(ClockTest, Attatch)
{
    const int num = 10;

    Clock clock;
    Counter counter_1;
    Counter counter_2;

    counter_1.attach(clock, 1);
    for (int i = 1; i <= num; ++i)
        clock.tick();
    EXPECT_EQ(counter_1(), num);

    counter_2.attach(clock, 1);
    for (int i = 1; i <= num; ++i)
        clock.tick();
    EXPECT_EQ(counter_1(), 2 * num);
    EXPECT_EQ(counter_2(), num);
}

TEST(ClockTest, Detatch)
{
    const int num = 10;

    Clock clock;
    Counter counter_1;
    Counter counter_2;
    Counter counter_3;

    counter_1.attach(clock, 1);
    counter_2.attach(clock, 1);
    counter_3.attach(clock, 1);
    for (int i = 1; i <= num; ++i)
        clock.tick();

    counter_3.detach();
    for (int i = 1; i <= num; ++i)
        clock.tick();

    counter_2.detach();
    for (int i = 1; i <= num; ++i)
        clock.tick();

    EXPECT_EQ(counter_1(), 3 * num);
    EXPECT_EQ(counter_2(), 2 * num);
    EXPECT_EQ(counter_3(), num);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
