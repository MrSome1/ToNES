
#include "Clock.h"

namespace tones {

/* Tickable */

Tickable::Tickable(int multiplier)
    : _multiplier(multiplier)
    , _clock(nullptr)
{

}

void Tickable::attach(Clock &clock)
{
    _clock = &clock;
    _clock->attach(this);
}

void Tickable::detach()
{
    _clock->detach(this);
    _clock = nullptr;
}

void Tickable::tick()
{
    for (int i = 0; i < _multiplier; ++i)
        _tick();
}

/* Clock */

void Clock::tick()
{
    for (auto it = _tickables.begin(); it != _tickables.end(); ++it)
        (*it)->tick();
}

void Clock::attach(Tickable *tickable)
{
    _tickables.push_back(tickable);
}

void Clock::detach(Tickable *tickable)
{
    for (auto it = _tickables.begin(); it != _tickables.end(); ++it) {
        if (*it == tickable) {
            _tickables.erase(it);
            break;
        }
    }
}

} // namespace tones
