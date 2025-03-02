
#include "MotherBoard.h"

#include <chrono>
#include <thread>

#include "Log.h"

namespace tones {

using namespace std::chrono;

static OutputPanel DefaultOuput;

/* DirectMemoryAccess */

DirectMemoryAccess::DirectMemoryAccess(uint16_t mmio, uint16_t dest, uint16_t len)
    : _mmio(mmio)
    , _dest(dest)
    , _len(len)
{

}

bool DirectMemoryAccess::contains(uint16_t addr) const
{
    return _mmio == addr;
}

void DirectMemoryAccess::read(uint16_t, uint8_t &buffer) const
{
    buffer = _page;
}

void DirectMemoryAccess::write(uint16_t, uint8_t data)
{
    _page = data;
    _addr = (uint16_t)_page << 8;
    for (int i = 0; i < _len; ++i) {
        _bus->read(_addr, _buff);
        _bus->write(_dest, _buff);
        ++_addr;
    }
}

/* MotherBoard */

MotherBoard::MotherBoard() 
    : _frequency(29781) // TODO: frequency depending on video type
    , _cpu(_mbus)
    , _ppu(_vbus, _mbus)
    , _odma(OAMDMA, ppu::OAMDATA, ppu::SpriteMemorySize)
    , _started(false)
    , _paused(true)
    , _output(&DefaultOuput)
{
    _pram.attach(_mbus);
    _vram.attach(_vbus);

    _odma.attach(_mbus);

    _cpu.attach(_clock, 1);
    _ppu.attach(_clock, 3);

    _ppu.setBlankHandler([&] () { _cpu.nmi(); });
}

void MotherBoard::insert(CartridgePtr &card)
{
    eject();
    _card = card;
    _card->attach(_mbus, _vbus);
    reset();
}

void MotherBoard::start()
{
    if ( _started || !_card)
        return;

    _started = true;
    _paused = false;

    run();
}

void MotherBoard::stop()
{
    _started = false;
    _paused = true;
    _output->onRegistersChanged();
}

void MotherBoard::pause()
{
    _paused = true;
    _output->onRegistersChanged();
}

void MotherBoard::resume()
{
    _paused = false;
}

void MotherBoard::step()
{
    _cpu.step();
    _output->onRegistersChanged();
}

bool MotherBoard::isStarted() const
{
    return _started;
}

bool MotherBoard::isPaused() const
{
    return _paused;
}

bool MotherBoard::isRunning() const
{
    return _started && !_paused;
}

void MotherBoard::setOutputPanel(OutputPanel &output)
{
    _output = &output;

    _ppu.setVideoOut([this] (int x, int y, uint32_t color) {
        _output->onVideoDotRendered(x, y, color);
    });

    _ppu.setFrameEnd([this] () {
        _output->onVideoFrameRendered();
    });
}

void MotherBoard::reset()
{
    _cpu.reset();
    _ppu.reset();

    _output->onRegistersChanged();
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

        for (uint32_t i = 0; i < _frequency; ++i) {
            if (_paused)
                break;
            _clock.tick();
        }

        auto duration = duration_cast<microseconds>(system_clock::now() - start);
        std::this_thread::sleep_for(cycle - duration);
    }
}

void MotherBoard::dumpCpuRegisters(MicroProcessor::Registers_t &regs)
{
    _cpu.dump(regs);
}

void MotherBoard::dumpPpuRegisters(PictureProcessingUnit::Registers_t &regs)
{
    _ppu.dump(regs);
}

void MotherBoard::dumpPpuPalettes(std::array<uint8_t, ppu::Palettes::PalettesSize> &colors)
{
    _ppu.dumpPalettes(colors);
}


} // namespace tones
