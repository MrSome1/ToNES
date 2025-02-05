
#include "Clock.h"

namespace tones {

/* Tickable */

Tickable::Tickable() : _clock(nullptr) {}

void Tickable::attach(Clock &clock, int multiplier)
{
    _clock = &clock;
    _clock->attach(this, multiplier);
}

void Tickable::detach()
{
    if (_clock) {
        _clock->detach(this);
        _clock = nullptr;
    }
}

/* Clock */

void Clock::tick()
{
    for (auto it = _tickables.begin(); it != _tickables.end(); ++it) {
        for (int i = 0; i < it->second; ++i)
            it->first->tick();
    }
}

void Clock::attach(Tickable *tickable, int multiplier)
{
    _tickables.emplace_back(std::make_pair(tickable, multiplier));
}

void Clock::detach(Tickable *tickable)
{
    for (auto it = _tickables.begin(); it != _tickables.end(); ++it) {
        if (it->first == tickable) {
            _tickables.erase(it);
            break;
        }
    }
}

} // namespace tones
