
#include <iostream>
#include <fstream>

#include "Cartridge.h"
#include "Log.h"

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
        LOG_ERROR() << "Failed to open ROM file " << path;
        return false;
    }

    if (!file.read(reinterpret_cast<char *>(&_header), sizeof(_header))) {
        LOG_ERROR() << "Failed to read header from " << path;
        return false;
    }

    if (!validate(_header)) {
        return false;
    }

    // Load PRG-ROM
    _prg_rom.resize(_header.prg * PrgRomBanckSize);
    if (!file.read((char*)_prg_rom.data(), _prg_rom.size())) {
        LOG_ERROR() << "Failed to load PRG-ROM from " << path;
        return false;
    }
    LOG_DEBUG() << "16KB PRG-ROM Banks: " << (int)_header.prg;

    // Load CHR-ROM
    if (_header.chr) {
        _chr_rom.resize(_header.chr * ChrRomBanckSize);
        if (!file.read((char*)_chr_rom.data(), _chr_rom.size())) {
            LOG_ERROR() << "Failed to load CHR-ROM from " << path;
            return false;
        }
    }
    LOG_DEBUG() << "8KB CHR-ROM Banks: " << (int)_header.chr;

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
        LOG_ERROR() << "Invalid iNES image";
        return false;
    }

    if (!header.prg) {
        LOG_ERROR() << "No PRG-ROM banks found";
        return false;
    }

    // TODO: Trainer

    if (header.ctrl1 & 0x4) {
        LOG_ERROR() << "Trainer is not supported yet";
        return false;
    }

    // TODO: Other bytes

    return true;
}

} // namespace rom

/* Cartridge */

Cartridge::Cartridge() {}

void Cartridge::attach(Bus &mbus, Bus &vbus)
{
    _prom->attach(mbus);
    _vrom->attach(vbus);
}

void Cartridge::detach()
{
    _prom->detach();
    _vrom->detach();
}

/* CartridgeFactory */

CartridgePtr CartridgeFactory::createCartridge(const std::string &path)
{
    auto reader = getReader(getFileExtension(path));

    if (!reader || !reader->load(path))
        return nullptr;

    auto card = CartridgePtr(new Cartridge);
    card->_reader.reset(reader);
    card->_mapper.reset(getMapper(reader->mapper()));
    card->_prom.reset(new ReadOnlyMemory(reader->prgRom()));
    card->_vrom.reset(new PatternTables(reader->chrRom()));

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
    return std::string::npos == it ? path : path.substr(it, path.size() - it);
}

} // namespace tones