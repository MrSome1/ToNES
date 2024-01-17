
#include <iostream>

#include "MotherBoard.h"
#include "Log.h"

namespace tones {

/* MotherBoard */

MotherBoard::MotherBoard() 
    : _cpu(_bus)
    , _ppu(_vbus)
{
    _ram.attach(_bus);
    _ppu.attach(_bus);
}

void MotherBoard::reset()
{
    _cpu.reset();
    dumpCPU();
}

void MotherBoard::start()
{
    std::cout << "First Run" << std::endl;

    for (int i = 0; i < 4; ++i) {
        _cpu.tick();
        dumpCPU();
    }
}

void MotherBoard::pause()
{
    /* TODO */
}

void MotherBoard::mount(CartridgePtr &card)
{
    _card = card;
    _card->attach(_bus);
}

void MotherBoard::unmount()
{
    _card->detach();
    _card.reset();
}

void MotherBoard::dumpCPU()
{
    MicroProcessor::Registers_t regs;
    _cpu.dump(regs);
    LOG_DEBUG() << std::hex
                << " A: 0x" <<  (int)regs.A
                << " X: 0x" <<  (int)regs.X
                << " Y: 0x" <<  (int)regs.Y
                << " S: 0x" <<  (int)regs.S
                << " P: 0x" <<  (int)regs.P
                << " PC: 0x" << (int)regs.PC
                << std::dec;
}

} // namespace tones