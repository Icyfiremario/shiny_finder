#ifndef SHINY_FINDER_PRNG_H
#define SHINY_FINDER_PRNG_H

// STD
#include <cstdint>

namespace PRNG
{
    inline uint32_t prng(const uint32_t seed)
    {
        return 0x41C64E6D * seed + 0x00006073;
    }

    inline uint32_t seed_to_val(const uint32_t seed)
    {
        return static_cast<uint16_t>(seed >> 16);
    }
}



#endif //SHINY_FINDER_PRNG_H
