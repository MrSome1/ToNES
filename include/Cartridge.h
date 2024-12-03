
#ifndef _TONES_CARTRIDGED_H_
#define _TONES_CARTRIDGED_H_

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "Device.h"

namespace tones {
namespace rom {

/* Cartridge File Format */
extern const char *iNES;
/* TODO: Other formats */

/* Abstraction of game cartridges */
class CartridgeReader
{
public:

    CartridgeReader();
    virtual ~CartridgeReader() = default;

    //! Load from a file
    virtual bool load(const std::string &path) = 0;

    virtual int mapper() const = 0;

    //! Get the content of PRG-ROM
    virtual const std::vector<uint8_t> &prgRom() const;

    //! Get the content of CHR-ROM
    virtual const std::vector<uint8_t> &chrRom() const;

protected:

    std::vector<uint8_t> _prg_rom; // content of PRG-ROM

    std::vector<uint8_t> _chr_rom; // content of CHR-ROM
};

class iNESReader: public CartridgeReader
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
                             // bit 4-7: four lower bits of ROM Mapper Type
        uint8_t ctrl2;       // bit 0: 1 for VS-System cartridges
                             // bit 1-3: reserved, must be zeroes
                             // bit 4-7: four higher bits of ROM Mapper Type
        uint8_t ram;         // number of 8kB RAM banks
        uint8_t video;       // bit 0: 1 = PAL, 0 = NTSC
                             // bit 1-7: reserved, must be zeroes
        uint8_t reserved[6]; // reserved bytes, must be zeroes
    } Header_t;

public:

    iNESReader();

    bool load(const std::string &path) override;

    int mapper() const override;

protected:

    bool validate(const Header &header);

private:

    Header_t _header;
};

} // namespace rom

class CartridgeFactory;

/**
 * @brief Loader of Cartridges
 */
class Cartridge
{
public:

    /** Attach to system bus
     * @param vbus bus of the ppu
     * @param mbus bus of the cpu
     */
    void attach(Bus &mbus, Bus &vbus);

    //! Detach from system bus
    void detach();

protected:

    Cartridge();
    Cartridge(Cartridge&) = delete;

private:

    friend class CartridgeFactory;

    std::unique_ptr<ReadOnlyMemory> _prom;

    std::unique_ptr<PatternTables> _vrom;

    std::unique_ptr<rom::CartridgeReader> _reader;

    std::unique_ptr<MemoryManagementController> _mapper;
};

typedef std::shared_ptr<Cartridge> CartridgePtr;

class CartridgeFactory
{

public:

    static CartridgePtr createCartridge(const std::string &path);

protected:

    static rom::CartridgeReader *getReader(const std::string &format);

    static MemoryManagementController *getMapper(int mapper);

    static const std::string getFileExtension(const std::string &path);
};

} // namespace tones

#endif // _TONES_CARTRIDGED_H_