#ifndef _TONES_UTILS_H_
#define _TONES_UTILS_H_

#include <array>
#include <cinttypes>

#define GET_BIT(bitwise, bit) \
    (std::get<static_cast<int>(bit)>(bitwise.bits))

#define SET_BIT(bitwise, bit, val) \
    (std::get<static_cast<int>(bit)>(bitwise.bits) = val)

namespace tones {
namespace reg {

const int RegisterBitCount = 8U;
const int AddressBitCount = 16U;

inline bool getBit(uint8_t byte, int bit);
inline void setBit(uint8_t &byte, int bit, bool val);

/* Bit Addressing Register */
typedef struct Bitwize {

    uint8_t value;
    std::array<bool, RegisterBitCount> bits;

    void setBit(int bit, bool val)
    {
        tones::reg::setBit(value, bit, val);
        bits.at(bit) = val;
    }

    void operator=(uint8_t val)
    {
        value = val;
        for (int i = 0; i < RegisterBitCount; ++i)
            bits[i] = tones::reg::getBit(val, i);
    }

    void operator=(const Bitwize &other)
    {
        value = other.value;
        bits = other.bits;
    }

} Bitwise_t;

inline void setBit(uint8_t &byte, int bit, bool val)
{
    uint8_t mask = 0x01 << bit;    
    val ? byte |= mask : byte &= ~mask;
}

inline bool getBit(uint8_t byte, int bit)
{
    return byte & (0x01 << bit);
}

inline void setMSB(uint16_t &value, uint8_t msb)
{
    value = (uint16_t)msb << 8 | (value & 0x00ff);
}

inline void setLSB(uint16_t &value, uint8_t lsb)
{
    value = value & 0xff00 | lsb;
}

inline void mergeTwoBytes(uint16_t &value, uint8_t msb, uint8_t lsb)
{
    value = (uint16_t)msb << 8 | lsb;
}

inline void splitTwoBytes(uint16_t value, uint8_t &msb, uint8_t &lsb)
{
    lsb = value & 0x00ff;
    msb = value & 0xff00 >> 8;
}

} // namespace reg
} // namespace tones

#endif // _TONES_UTILS_H_