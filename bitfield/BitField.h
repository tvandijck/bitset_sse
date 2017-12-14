#pragma once

#include <stdint.h>
#include <intrin.h>

class BitField
{
public:
    BitField();

    void Set(uint32_t idx);
    void Set(uint32_t idx, uint32_t numBits);

    void Clear(uint32_t idx);
    void Clear(uint32_t idx, uint32_t numBits);

    bool IsSet(uint32_t idx) const;
    int Find(uint32_t numBits) const;

    size_t size() const;
    const void* data() const;

private:
    __m128i m_data[16];
    uint8_t m_free[16];
};

inline size_t BitField::size() const
{
    return sizeof(m_data);
}

inline const void* BitField::data() const
{
    return m_data;
}

