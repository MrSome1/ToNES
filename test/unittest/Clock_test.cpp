
#include <gtest/gtest.h>

#include "Clock.h"

using namespace tones;

class Counter : public Tickable
{

public:

    int value = 0;

    Counter(int multiplier=1) : Tickable(multiplier) {}

    void _tick() { ++value; }

    int operator()() { return value; }
};

TEST(ClockTest, Tick)
{
    const int num = 10;
    const int mul = 5;

    Clock clock;
    Counter counter_1;
    Counter counter_2(mul);

    counter_1.attach(clock);
    counter_2.attach(clock);
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
    Counter counter_1(1);
    Counter counter_2(1);

    counter_1.attach(clock);
    for (int i = 1; i <= num; ++i)
        clock.tick();
    EXPECT_EQ(counter_1(), num);

    counter_2.attach(clock);
    for (int i = 1; i <= num; ++i)
        clock.tick();
    EXPECT_EQ(counter_1(), 2 * num);
    EXPECT_EQ(counter_2(), num);
}

TEST(ClockTest, Detatch)
{
    const int num = 10;

    Clock clock;
    Counter counter_1(1);
    Counter counter_2(1);
    Counter counter_3(1);

    counter_1.attach(clock);
    counter_2.attach(clock);
    counter_3.attach(clock);
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
