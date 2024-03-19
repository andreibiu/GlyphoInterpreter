/// ============================
/// @author Andrei Biu - Pislaru
/// @brief Implementations for functions under ArithmeticOps namespace
/// ============================

#ifndef ARITHMETIC_OPS_HPP
#define ARITHMETIC_OPS_HPP

#include "Typedefs.hpp"

/**
 * @namespace ArithmeticOps
 * @brief Contains functions for basic math operations
 *      implemented with inline assembly for maximum
 *      performance
 */
namespace ArithmeticOps
{
    INLINE dxword Multiply(xword lOperand, xword rOperand)
    {
        dxword result;
    #if defined(__i386__)
        asm (
            "mov %%eax, %2" "\n\t"
            "mul %3"        "\n\t"
            "mov %0, %%edx" "\n\t"
            "mov %1, %%eax"
            : "=r" (result.first), "=r" (result.second)
            : "r" (lOperand), "r" (rOperand)
            : "edx", "eax"
        );
    #elif defined(__x86_64__)
        asm (
            "mov %%rax, %2" "\n\t"
            "mul %3"        "\n\t"
            "mov %0, %%rdx" "\n\t"
            "mov %1, %%rax"
            : "=r" (result.first), "=r" (result.second)
            : "r" (lOperand), "r" (rOperand)
            : "rdx", "rax"
        );
    #endif
        return result;
    }

    INLINE dxword Divide(xword hDividend, xword lDividend, xword divisor)
    {
        dxword result;
    #if defined(__i386__)
        asm (
            "mov %%edx, %2" "\n\t"
            "mov %%eax, %3" "\n\t"
            "div %4"        "\n\t"
            "mov %0, %%eax" "\n\t"
            "mov %1, %%edx"
            : "=r" (result.first), "=r" (result.second)
            : "r" (hDividend), "r" (lDividend), "r" (divisor)
            : "edx", "eax"
        );
    #elif defined(__x86_64__)
        asm (
                "mov %%rdx, %2"  "\n\t"
                "mov %%rax, %3" "\n\t"
                "div %4"        "\n\t"
                "mov %0, %%rax" "\n\t"
                "mov %1, %%rdx"
                : "=r" (result.first), "=r" (result.second)
                : "r" (hDividend), "r" (lDividend), "r" (divisor)
                : "rdx", "rax"
            );
    #endif   
        return result;
    }

    INLINE xword Log2Floor(xword value)
    {
        xword result;
        asm (
            "mov %0, 0" "\n\t"
            "bsr %0, %1"
            : "=&r" (result)
            : "r" (value)
            :
        );
        return result;
    }

    INLINE xword Log2Ceil(xword value)
    {
        xword result1 = Log2Floor(value);
        xword result2;
        asm (
            "mov %0, 0" "\n\t"
            "bsf %0, %1"
            : "=&r" (result2)
            : "r" (value)
            :
        );
        return result1 + (result1 != result2);
    }
};

#endif  // ARITHMETIC_OPS_HPP
