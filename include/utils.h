#ifndef _TONES_UTILS_H_
#define _TONES_UTILS_H_

#include <cinttypes>

namespace tones {

inline void setBit(uint8_t &byte, int bit, bool val) {
    uint8_t mask = 0x01 << bit;    
    val ? byte |= mask : byte &= ~mask;
}

inline bool getBit(uint8_t byte, int bit) {
    return byte & (0x01 << bit);
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

} // namespace tones

#endif // _TONES_UTILS_H_