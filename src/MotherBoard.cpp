
#include "MotherBoard.h"

#include <chrono>
#include <thread>

#include "Log.h"

namespace tones {

using namespace std::chrono;

static OutputPanel DefaultOuput;

/* MotherBoard */

MotherBoard::MotherBoard() 
    : _frequency(29781) // TODO: frequency depending on video type
    , _cpu(_mbus)
    , _ppu(_vbus, _mbus)
    , _started(false)
    , _running(false)
    , _output(&DefaultOuput)
{
    _pram.attach(_mbus);
    _vram.attach(_vbus);

    _cpu.attach(_clock);
    _ppu.attach(_clock);
}

void MotherBoard::insert(CartridgePtr &card)
{
    eject();
    _card = card;
    _card->attach(_mbus, _vbus);
}

void MotherBoard::start()
{
    if ( _started || !_card)
        return;

    _started = true;
    _running = true;

    reset();
    run();
}

void MotherBoard::stop()
{
    _started = false;
    _running = false;
}

void MotherBoard::pause()
{
    _running = false; 
}

void MotherBoard::resume()
{
    _running = true;
}

bool MotherBoard::isStarted() const
{
    return _started;
}

bool MotherBoard::isRunning() const
{
    return _running;
}

void MotherBoard::setOutputPanel(OutputPanel &output)
{
    _output = &output;

    _ppu.setVideoOut([this] (int x, int y, const RGB &color) {
        _output->onVideoDotRendered(x, y, color);
    });

    _ppu.setFrameEnd([this] () {
        _output->onVideoFrameRendered();
    });
}

void MotherBoard::reset()
{
    _cpu.reset();
    debugCpu();
}

void MotherBoard::eject()
{
    if (_card) {
        _card->detach();
        _card.reset();
    }
}

void MotherBoard::run()
{
    std::chrono::microseconds cycle(16667); // 1/60 s

    while (_started) { // loop on video frame
        auto start = system_clock::now();

        if (_running) {
            for (uint32_t i = 0; i < _frequency; ++i) {
                if (_running) {
                    _clock.tick(); // TODO: too slow
                    debugCpu();
                } else {
                    break;
                }
            }
        }

        auto duration = duration_cast<microseconds>(system_clock::now() - start);
        std::this_thread::sleep_for(cycle - duration);
    }
}

void MotherBoard::debugCpu()
{
    _cpu.dump(_output->_registers);
    _output->onCpuStepped();
}

} // namespace tones