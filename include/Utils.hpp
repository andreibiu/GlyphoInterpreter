/// ============================
/// @author Andrei Biu - Pislaru
/// @brief Implementations for functions under Utils namespace
/// ============================

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include "Typedefs.hpp"

/**
 * @namespace Utils
 * @brief Contains functions with miscellaneous uses
 */
namespace Utils
{
    INLINE bool IsInvalidDigit(char digit, byte base)
    {
        return base > 10 ?
            (digit < '0' || digit > '9' && (digit < 'A' || digit > ('A' + base - 11))) :
            (digit < '0' || digit > '9');
    }
    
    INLINE byte ToNumber(char digit)
    {
        return digit - ((digit < 58) ? 48 : 55);
    }

    INLINE char ToDigit(byte number)
    {
        return number + ((number < 10) ? 48 : 55);
    }

    INLINE xword Parse(std::string string, byte base)
    {
        if constexpr (sizeof(xword) == 4)       // 32-bit architecture
            return std::stoul(string, nullptr, base);
        else if constexpr (sizeof(xword) == 8)  // 64-bit architecture
            return std::stoull(string, nullptr, base);
        else                                    // (o_0)
            return 0;
    }
}

#endif  // UTILS_HPP
