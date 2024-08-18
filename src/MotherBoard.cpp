
#include "MotherBoard.h"

#include <chrono>
#include <thread>

#include "Log.h"

namespace tones {

using namespace std::chrono;

/* MotherBoard */

MotherBoard::MotherBoard() 
    : _frequency(29781) // TODO: frequency depending on video type
    , _cpu(_mbus)
    , _ppu(_vbus, _mbus)
    , _started(false)
    , _running(false)
{
    _pram.attach(_mbus);
    _vram.attach(_vbus);

    _cpu.attach(_clock);
    _ppu.attach(_clock); // TODO: crash here
}

void MotherBoard::reset()
{
    _cpu.reset();
    debugCpu();
}

void MotherBoard::start()
{
    if (!_started) {
        _started = true;
        _running = true;
        run();
    }
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

void MotherBoard::insert(CartridgePtr &card)
{
    _card = card;
    _card->attach(_mbus, _vbus);
}

void MotherBoard::eject()
{
    _card->detach();
    _card.reset();
}

void MotherBoard::setVideoOut(VideoOut output)
{
    _ppu.setVideoOut(output);
}

void MotherBoard::setFrameEnd(FrameEnd flush)
{
    _ppu.setFrameEnd(flush);
}

void MotherBoard::setCpuViewer(CpuViewer viewer)
{
    if (!viewer)
        return;
    _cpuViewer = viewer;
}

void MotherBoard::debugCpu()
{
    if (_cpuViewer) {
        MicroProcessor::Registers_t regs;
        _cpu.dump(regs);
        _cpuViewer(regs);
    }
}

} // namespace tones