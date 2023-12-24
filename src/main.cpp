
#include <iostream>

#include "Cartridge.h"
#include "MotherBoard.h"

int main(int argc, char* argv[]) {
    const char *filename = "../test/example.nes";

    auto card = tones::CartridgeFactory::createCartridge(filename);

    tones::MotherBoard board;
    board.reset();

    board.mount(card);
    board.start();

    return 0;
}