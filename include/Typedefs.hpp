/// ============================
/// @author Andrei Biu - Pislaru
/// @brief General aliases & typedefs
/// ============================

#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#include <cstdint>
#include <utility>

#define INLINE [[gnu::always_inline]] inline

using byte   = unsigned char;            // 8 bit unsigned integral type
using xword  = std::size_t;              // maximum width unsigned integral type for math & mem
using dxword = std::pair<xword, xword>;  // structure for double-width xwords

template <typename TResult, typename... TArgs> using Function = TResult (*)(TArgs...);

#endif  // TYPEDEFS_HPP
