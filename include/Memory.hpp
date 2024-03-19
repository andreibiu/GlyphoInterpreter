/// ============================
/// @author Andrei Biu - Pislaru
/// @brief Implementation of Memory "static" class
/// ============================

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <algorithm>
#include <deque>
#include <iostream>

#include "Integer.hpp"

/**
 * @class Memory
 * @brief Implements the Glypho stack-like memory using the STL's std::dequeue
 * @note All members are static to simplify the singleton pattern
 */
class Memory
{   
    /// @defgroup Static Fields
    private:
        static std::deque<Integer> data;

    /// @defgroup Static Methods
    public:
        static void Push(const Integer& integer);
        static void Push(Integer&& integer);
        static Integer Pop();
        static Integer& Peek();
        static void RemoveTop();
        static void ShiftUp();
        static void ShiftDown();
        static void DuplicateTop();
        static void SwapTop();
        static void Dump();
};

/// @ingroup Static Fields
inline std::deque<Integer> Memory::data;

/// @ingroup Static Methods
/// @{

INLINE void Memory::Push(const Integer& integer)
{
    data.push_front(integer);
}

INLINE void Memory::Push(Integer&& integer)
{
    data.push_front(std::move(integer));
}

INLINE void Memory::RemoveTop()
{
    if (data.empty())
        throw 0;

    data.pop_front();
}

INLINE Integer Memory::Pop()
{
    if (data.empty())
        throw 0;

    Integer value = std::move(data.front());
    data.pop_front();
    return value;
}

INLINE Integer& Memory::Peek()
{
    if (data.empty())
        throw 0;

    return data.front();
}

INLINE void Memory::ShiftUp()
{
    if (data.empty())
        throw 0;

    Integer value = std::move(data.front());
    data.pop_front();
    data.push_back(std::move(value));
}

INLINE void Memory::ShiftDown()
{
    if (data.empty())
        throw 0;

    Integer value = std::move(data.back());
    data.pop_back();
    data.push_front(std::move(value));
}

INLINE void Memory::DuplicateTop()
{
    if (data.empty())
        throw 0;

    data.push_front(data.front());
}

INLINE void Memory::SwapTop()
{
    if (data.size() < 2)
        throw 0;

    auto iterator = data.begin();
    std::iter_swap(iterator, iterator + 1);
}

INLINE void Memory::Dump()
{
    for (xword i = 0; i < data.size(); ++i)
        std::cout << "MEM_DUMP [" << i << "]: " << data[i].ToString() << std::endl;
    std::cout << std::endl;
}

/// @}

#endif  // MEMORY_HPP
