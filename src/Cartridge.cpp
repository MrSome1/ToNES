
#include <iostream>
#include <fstream>

#include "Cartridge.h"

namespace tones {
namespace rom {

const std::vector<uint8_t> &Cartridge::getPrgRom() const
{
    return _prg_rom;
}

const std::vector<uint8_t> &Cartridge::getChrRom() const
{
    return _chr_rom;
}

iNESCartridge::iNESCartridge(): Cartridge() {}

bool iNESCartridge::loadFile(const std::string &path)
{
    Header header;
    std::ifstream file(path, std::ios_base::binary | std::ios_base::in);

    if (!file) {
        std::cerr << "Could not open ROM file from path: " << path << std::endl;
        return false;
    }

    if (!file.read(reinterpret_cast<char *>(&header), 16)) {
        std::cerr << "Reading iNES header failed." << std::endl;
        return false;
    }

    if (!validate(header)) {
        return false;
    }

    // Load PRG-ROM
    _prg_rom.resize(header.prg * PrgRomBanckSize);
    if (!file.read((char*)_prg_rom.data(), _prg_rom.size())) {
        std::cerr << "Reading PRG-ROM failed." << std::endl;
        return false;
    }

    // Load CHR-ROM
    if (header.chr) {
        _chr_rom.resize(header.chr * ChrRomBanckSize);
        if (!file.read((char*)_chr_rom.data(), _chr_rom.size())) {
            std::cerr << "Reading CHR-ROM failed." << std::endl;
            return false;
        }
    }

    return true;
}

bool iNESCartridge::validate(const Header &header)
{
    if (header.magic != MagicNumber) {
        std::cerr << "Not a valid iNES image" << std::endl;
        return false;
    }

    if (!header.prg) {
        std::cerr << "ROM has no PRG-ROM banks. Loading ROM failed." << std::endl;
        return false;
    }

    // TODO: Trainer

    if (header.ctrl1 & 0x4) {
        std::cerr << "Trainer is not supported." << std::endl;
        return false;
    }

    // TODO: Other bytes

    return true;
}

} // namespace rom
} // namespace tones