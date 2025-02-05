
#ifndef _TONES_CLOCK_H_
#define _TONES_CLOCK_H_

#include <cinttypes>
#include <vector>

namespace tones {

class Clock;

class Tickable
{

public:

    Tickable();

    virtual void tick() = 0;

    void attach(Clock &clock, int multiplier);

    void detach();

private:

    Clock *_clock;
};

class Clock
{

public:

    void tick();

protected:

    friend class Tickable;

    void attach(Tickable *tickable, int multiplier);

    void detach(Tickable *tickable);

private:

    std::vector<std::pair<Tickable*, int>> _tickables;
};

} // namespace tones

#endif // _TONES_CLOCK_H_
