
#ifndef _TONES_CLOCK_H_
#define _TONES_CLOCK_H_

#include <cinttypes>
#include <vector>

namespace tones {

class Clock;

class Tickable
{

public:

    Tickable(int multiplier=1);

    void attach(Clock &clock);

    void detach();

    void tick();

protected:

    virtual void _tick() = 0;

private:

    int _multiplier;

    Clock *_clock;
};

class Clock
{

public:

    void tick();

protected:

    friend class Tickable;

    void attach(Tickable *tickable);

    void detach(Tickable *tickable);

private:

    std::vector<Tickable*> _tickables;
};

} // namespace tones

#endif // _TONES_CLOCK_H_
