
#include <iostream>
#include <fstream>

#include "Cartridge.h"

namespace tones {
namespace rom {

/* Cartridge File Format */
const char *iNES = ".nes";
/* TODO: Other formats */

/* CartridgeReader */

CartridgeReader::~CartridgeReader() {}

const std::vector<uint8_t> &CartridgeReader::prgRom() const
{
    return _prg_rom;
}

const std::vector<uint8_t> &CartridgeReader::chrRom() const
{
    return _chr_rom;
}

/* iNESReader */

iNESReader::iNESReader(): CartridgeReader() {}

bool iNESReader::load(const std::string &path)
{
    std::ifstream file(path, std::ios_base::binary | std::ios_base::in);

    if (!file) {
        std::cerr << "Could not open ROM file from path: " << path << std::endl;
        return false;
    }

    if (!file.read(reinterpret_cast<char *>(&_header), 16)) {
        std::cerr << "Reading iNES header failed." << std::endl;
        return false;
    }

    if (!validate(_header)) {
        return false;
    }

    // Load PRG-ROM
    _prg_rom.resize(_header.prg * PrgRomBanckSize);
    if (!file.read((char*)_prg_rom.data(), _prg_rom.size())) {
        std::cerr << "Reading PRG-ROM failed." << std::endl;
        return false;
    }

    // Load CHR-ROM
    if (_header.chr) {
        _chr_rom.resize(_header.chr * ChrRomBanckSize);
        if (!file.read((char*)_chr_rom.data(), _chr_rom.size())) {
            std::cerr << "Reading CHR-ROM failed." << std::endl;
            return false;
        }
    }

    return true;
}

int iNESReader::mapper() const
{
    // TODO: Use enum?
    return (_header.ctrl2 & 0xf0) | ((_header.ctrl1 & 0xf0) >> 4);
}

bool iNESReader::validate(const Header &header)
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

/* Cartridge */

Cartridge::Cartridge() {}

void Cartridge::attach(Bus &bus)
{
    _rom->attach(bus);
}

/* CartridgeFactory */

CartridgePtr CartridgeFactory::createCartridge(const std::string &path)
{
    auto reader = getReader(getFileExtension(path));

    if (!reader->load(path))
        return nullptr;

    auto mapper = getMapper(reader->mapper());

    auto rom = new ReadOnlyMemory(reader->prgRom());

    auto card = CartridgePtr(new Cartridge);
    card->_rom = std::unique_ptr<ReadOnlyMemory>(rom);
    card->_reader = std::unique_ptr<rom::CartridgeReader>(reader);
    card->_mapper = std::unique_ptr<MemoryManagementController>(mapper);

    return card;
}

rom::CartridgeReader *CartridgeFactory::getReader(const std::string &format)
{
    /* TODO: Other formats */
    if (!format.compare(rom::iNES)) {
        return new rom::iNESReader;
    } else {
        return nullptr;
    }
}

MemoryManagementController *CartridgeFactory::getMapper(int mapper)
{
    return nullptr; // TODO
}

const std::string CartridgeFactory::getFileExtension(const std::string &path)
{
    auto it = path.rfind('.');
    it = std::string::npos == it ? 0 : it;
    return path.substr(it, path.size() - it);
}

} // namespace tones