
#ifndef _TONES_CARTRIDGED_H_
#define _TONES_CARTRIDGED_H_

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

namespace tones {
namespace rom {

const int PrgRomBanckSize = 0x4000; // 16kB
const int ChrRomBanckSize = 0x2000; // 8kB

/* Abstraction of game cartridges */
class Cartridge
{
public:

    //! Load from a file
    virtual bool loadFile(const std::string &path) = 0;

    //! Get the content of PRG-ROM
    virtual const std::vector<uint8_t> &getPrgRom() const;

    //! Get the content of CHR-ROM
    virtual const std::vector<uint8_t> &getChrRom() const;

protected:

    std::vector<uint8_t> _prg_rom; // content of PRG-ROM

    std::vector<uint8_t> _chr_rom; // content of CHR-ROM
};

class iNESCartridge: public Cartridge
{
    static const uint32_t MagicNumber = 0x1a53454e;  // "NES^Z" 

    /* Header of an iNES file */
    typedef struct Header {
        uint32_t magic;      // expected string "NES^Z"
        uint8_t prg;         // number of 16kB PRG-ROM banks
        uint8_t chr;         // number of 8kB CHR-ROM banks
        uint8_t ctrl1;       // bit 0: mirroring, 1 = vertical, 0 = horizontal
                             // bit 1: 1 for SRAM at $6000-$7FFF
                             // bit 2: 1 for a 512-byte trainer at $7000-$71FF
                             // bit 3: 1 for a four-screen VRAM layout
                             // bit 4-7: four higher bits of ROM Mapper Type
        uint8_t ctrl2;       // bit 0: 1 for VS-System cartridges
                             // bit 1-3: reserved, must be zeroes
                             // bit 4-7: four higher bits of ROM Mapper Type
        uint8_t ram;         // number of 8kB RAM banks
        uint8_t video;       // bit 0: 1 = PAL, 0 = NTSC
                             // bit 1-7: reserved, must be zeroes
        uint8_t reserved[6]; // reserved bytes, must be zeroes
    } Header_t;

public:

    iNESCartridge();

    bool loadFile(const std::string &path);

protected:

    bool validate(const Header &header);
};

} // namespace rom

/**
 * @brief Loader of Cartridges
 */
class CartridgeReader
{

private:

    std::shared_ptr<rom::Cartridge> _card;
};

} // namespace tones

#endif // _TONES_CARTRIDGED_H_