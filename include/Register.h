#ifndef _TONES_REGISTER_H_
#define _TONES_REGISTER_H_

#include <array>
#include <cinttypes>

#define GET_BIT(bitwise, bit) \
    tones::reg::getBit(bitwise, static_cast<int>(bit))

#define SEL_BIT(bitwise, bit) \
    tones::reg::setBit(bitwise, static_cast<int>(bit), true)

#define CLR_BIT(bitwise, bit) \
    tones::reg::setBit(bitwise, static_cast<int>(bit), false)

#define SET_BIT(bitwise, bit, val) \
    tones::reg::setBit(bitwise, static_cast<int>(bit), val)

namespace tones {
namespace reg {

const int RegisterBitCount = 8U;
const int AddressBitCount = 16U;

/* Bit Addressing Register */
typedef uint8_t Bitwise_t;

template <typename T>
struct Base {

    T value;

    operator T() { return value; }

    // TODO: Why this can not be inherited?
    T operator =(T other) { return value = other; }

    T operator >>(int count) { return value >> count; }
    T operator <<(int count) { return value << count; }
};

/**
 * @brief Shift Register
 * 
 * Shift one bit one time
 */
typedef struct Shift: public Base<uint8_t> {

    //! Parallel-to-Serial left shift
    bool leftShift()
    {
        bool val = value & 0x80;
        value <<= 1;
        return val;
    }

    // Serial-to-Parallel left shift
    uint8_t leftShift(bool val)
    {
        value <<= 1;
        value |= val ? 0x01 : 0x00;
        return value;
    }

    //! Parallel-to-Serial right shift
    bool rightShift()
    {
        bool val = value & 0x01;
        value >>= 1;
        return val;
    }

    // Serial-to-Parallel right shift
    uint8_t rightShift(bool val)
    {
        value >>= 1;
        value |= val ? 0x80 : 0x00;
        return value;
    }

    uint8_t operator <<(Shift &other)
    {
        return leftShift(other.leftShift());
    }

    uint8_t operator >>(Shift &other)
    {
        return other.rightShift(rightShift());
    }

    uint8_t operator =(uint8_t other) { return value = other; }

} Shift_t;

/**
 * @brief Cycle Register
 * 
 * Used to store a count
 */
typedef struct Cycle: public Base<uint16_t> {

    uint16_t limit;

    void reset(uint16_t bound)
    {
        value = 0;
        limit = bound;
    }

    bool empty()
    {
        return !value;
    }

    bool full()
    {
        return value == limit;
    }

    bool overflow()
    {
        return value >= limit;
    }

    uint16_t operator++()
    {
        return overflow() ? value = 0 : ++value;
    }

    uint16_t operator--()
    {
        return empty() ? value = limit : --value;
    }

    uint16_t operator =(uint16_t other) { return value = other; }

} Cycle_t;

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

inline bool isOdd(uint16_t value)
{
    return value & 0x01;
}

inline bool isEven(uint16_t value)
{
    return !isOdd(value);
}

} // namespace reg
} // namespace tones

#endif // _TONES_REGISTER_H_