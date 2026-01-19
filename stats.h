#ifndef SHINY_FINDER_STATS_H
#define SHINY_FINDER_STATS_H

// STD
#include <cstdint>

// Local
#include "prng.h"


namespace Stats
{

    constexpr std::string natures[] = {
        "Hardy", "Lonely", "Brave", "Adamant", "Naughty",
        "Bold", "Docile", "Relaxed", "Impish", "Lax",
        "Timid", "Hasty", "Serious", "Jolly", "Naive",
        "Modest", "Mild", "Quiet", "Bashful", "Rash",
        "Calm", "Gentle", "Sassy", "Careful", "Quirky"
    };

    struct ShinyResult
    {
        uint32_t seed{};
        uint32_t PID{};
        uint32_t natureID{};
        int IVs[6]{};
    };

    /// @brief Generates IVs for PID methods STATIC-1 and WILD-1
    inline void get_method1_ivs(const uint32_t s1, ShinyResult& res)
    {
        const uint32_t s2 = PRNG::prng(s1);
        const uint32_t s3 = PRNG::prng(s2);
        const uint32_t s4 = PRNG::prng(s3);

        const uint16_t iv_word1 = PRNG::seed_to_val(s3);
        const uint16_t iv_word2 = PRNG::seed_to_val(s4);

        res.IVs[0] = iv_word1 & 0x1F;         // HP
        res.IVs[1] = iv_word1 >> 5 & 0x1F;    // ATK
        res.IVs[2] = iv_word1 >> 10 & 0x1F;   // DEF

        res.IVs[3] = iv_word2 & 0x1F;         // SPE
        res.IVs[4] = iv_word2 >> 5 & 0x1F;    // SPA
        res.IVs[5] = iv_word2 >> 10 & 0x1F;   // SPD
    }

    /// @brief Generates IVs for PID method WILD-2
    inline void get_method2_ivs(const uint32_t s1, ShinyResult& res)
    {
        const uint32_t s2 = PRNG::prng(s1);
        const uint32_t vblank_advance = PRNG::prng(s2);
        const uint32_t s3 = PRNG::prng(vblank_advance);
        const uint32_t s4 = PRNG::prng(s3);

        const uint16_t iv_word1 = PRNG::seed_to_val(s3);
        const uint16_t iv_word2 = PRNG::seed_to_val(s4);

        res.IVs[0] = iv_word1 & 0x1F;         // HP
        res.IVs[1] = iv_word1 >> 5 & 0x1F;    // ATK
        res.IVs[2] = iv_word1 >> 10 & 0x1F;   // DEF

        res.IVs[3] = iv_word2 & 0x1F;         // SPE
        res.IVs[4] = iv_word2 >> 5 & 0x1F;    // SPA
        res.IVs[5] = iv_word2 >> 10 & 0x1F;   // SPD
    }

    /// @brief Generates IVs for PID methods STATIC-4 and WILD-4
    inline void get_method4_ivs(const uint32_t s1, ShinyResult& res)
    {
        const uint32_t s2 = PRNG::prng(s1);
        const uint32_t s3 = PRNG::prng(s2);
        const uint32_t vblank_advance = PRNG::prng(s3);
        const uint32_t s4 = PRNG::prng(vblank_advance);

        const uint16_t iv_word1 = PRNG::seed_to_val(s3);
        const uint16_t iv_word2 = PRNG::seed_to_val(s4);

        res.IVs[0] = iv_word1 & 0x1F;         // HP
        res.IVs[1] = iv_word1 >> 5 & 0x1F;    // ATK
        res.IVs[2] = iv_word1 >> 10 & 0x1F;   // DEF

        res.IVs[3] = iv_word2 & 0x1F;         // SPE
        res.IVs[4] = iv_word2 >> 5 & 0x1F;    // SPA
        res.IVs[5] = iv_word2 >> 10 & 0x1F;   // SPD
    }
}

#endif // SHINY_FINDER_STATS_H