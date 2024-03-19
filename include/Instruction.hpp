/// ============================
/// @author Andrei Biu - Pislaru
/// @brief Implementation for Glypho instructions
/// ============================

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdlib>
#include <iostream>
#include <string>
#include "Environment.hpp"
#include "Memory.hpp"

/**
 * @class Instruction
 * @brief Implements a Glypho instructions; an instruction
 *        is composed from multiple lower-level operations
 *        provided by other components of the interpreter
 *        and returns an offset that is used to select the
 *        next instruction; it also has a type that is
 *        convertible to the Glypho .glh symbolic notation 
 */
class Instruction
{
    /// @defgroup Nested Types
    public:
        enum class Type : char
        {
            Halt     = '?',
            Nop      = 'n',
            Input    = 'i',
            Rot      = '>',
            Swap     = '\\',
            Push     = '1',
            RRot     = '<',
            Dup      = 'd',
            Add      = '+',
            Lbrace   = '[',
            Output   = 'o',
            Multiply = '*',
            Execute  = 'e',
            Negate   = '-',
            Pop      = '!',
            Rbrace   = ']'
        };

    /// @defgroup Private Static Methods
    private:
        static long HaltOperations(long);
        static long NopOperations(long);
        static long InputOperations(long);
        static long RotOperations(long);
        static long SwapOperations(long);
        static long PushOperations(long);
        static long RRotOperations(long);
        static long DupOperations(long);
        static long AddOperations(long);
        static long LbraceOperations(long);
        static long OutputOperations(long);
        static long MultiplyOperations(long);
        static long ExecuteOperations(long);
        static long NegateOperations(long);
        static long PopOperations(long);
        static long RbraceOperations(long);

    /// @defgroup Public Static Methods
    public:
        template <typename T> INLINE static Instruction GetNext(T* symbols);
    
    /// @defgroup Fields
    private:
        Type type;
        long offset;
        Function<long, long> operations;

    /// @defgroup Constructors
    public:        
        Instruction(Type type, long offset, Function<long, long> operations) :
            type(type), offset(offset), operations(operations) {}

    /// @defgroup Getters & Setters
    public:
        INLINE Type Type() const { return type; }
        INLINE long Offset() const { return offset; }
        INLINE void Offset(long offset) { this->offset = offset; }

    /// @defgroup Operators
    public:
        INLINE long operator () () const { return operations(offset); }
};


/// @ingroup Private Static Methods 

INLINE long Instruction::HaltOperations(long)
{
    Environment::ExitNormal(); return 0;
}

INLINE long Instruction::NopOperations(long)
{
    return 1;
}

INLINE long Instruction::InputOperations(long)
{
    std::string input;
    std::cin >> input;
    Memory::Push(Integer(input, Environment::Base()));
    return 1;
}

INLINE long Instruction::RotOperations(long)
{
    Memory::ShiftUp();
    return 1;
}

INLINE long Instruction::SwapOperations(long)
{
    Memory::SwapTop();
    return 1;
}

INLINE long Instruction::PushOperations(long)
{
    Memory::Push(Integer(1));
    return 1;
}

INLINE long Instruction::RRotOperations(long)
{
    Memory::ShiftDown();
    return 1;
}

INLINE long Instruction::DupOperations(long)
{
    Memory::DuplicateTop();
    return 1;
}

INLINE long Instruction::AddOperations(long)
{
    Integer value0 = Memory::Pop(),
            value1 = Memory::Pop();
    Memory::Push(value0 + value1);
    return 1;
}

INLINE long Instruction::LbraceOperations(long offset)
{
    if (Memory::Peek().IsZero())
        return offset;
    else
        return 1;
}

INLINE long Instruction::OutputOperations(long)
{
    std::cout << Memory::Pop().ToString(Environment::Base()) << std::endl;
    return 1;
}

INLINE long Instruction::MultiplyOperations(long)
{
    Integer value0 = Memory::Pop(),
            value1 = Memory::Pop();
    Memory::Push(value0 * value1);
    return 1;
}

INLINE long Instruction::ExecuteOperations(long)
{
    Integer symbols[] = {
        Memory::Pop(),
        Memory::Pop(),
        Memory::Pop(),
        Memory::Pop(),
    };
    Instruction instruction = GetNext(symbols);
    if (
        instruction.Type() != Instruction::Type::Lbrace &&
        instruction.Type() != Instruction::Type::Rbrace
    )
        return instruction();
    throw 1;
}

INLINE long Instruction::NegateOperations(long)
{
    Memory::Peek().Negate();
    return 1;
}

INLINE long Instruction::PopOperations(long)
{
    Memory::RemoveTop();
    return 1;
}

INLINE long Instruction::RbraceOperations(long offset)
{
    return offset;
}


/// @ingroup Public Static Methods

/**
 * @brief Get (parses) the next instruction from an array
 *      of symbols that are equality - comparable  
 */
template <typename T> INLINE Instruction Instruction::GetNext(T* symbols)
{
    // An equality - comparator network that computes
    // the instruction code with independency of the symbols
    // used from one instruction to the next
    auto getCode = [] (T* symbols) -> byte {
        bool flag0 = symbols[1] != symbols[0],
             flag1 = symbols[2] != symbols[0],
             flag2 = flag1 && symbols[2] != symbols[1],
             flag3 = flag0 && flag2,
             flag4 = symbols[3] != symbols[0],
             flag5 = symbols[3] != symbols[1],
             flag6 = flag0 && flag4 && flag5,
             flag7 = flag2 && flag4 && flag5 && symbols[3] != symbols[2];
        return 9 * flag0 + 3 * (flag1 + flag3) + (flag4 + flag6 + flag7);
    };

    switch (getCode(symbols))
    {
        case  0: return Instruction(Instruction::Type::Nop, 1, NopOperations);
        case  1: return Instruction(Instruction::Type::Input, 1, InputOperations);
        case  3: return Instruction(Instruction::Type::Rot, 1, RotOperations);
        case  4: return Instruction(Instruction::Type::Swap, 1, SwapOperations);
        case  5: return Instruction(Instruction::Type::Push, 1, PushOperations);
        case  9: return Instruction(Instruction::Type::RRot, 1, RRotOperations);
        case 10: return Instruction(Instruction::Type::Dup, 1, DupOperations);
        case 11: return Instruction(Instruction::Type::Add, 1, AddOperations);
        case 12: return Instruction(Instruction::Type::Lbrace, 1, LbraceOperations);
        case 13: return Instruction(Instruction::Type::Output, 1, OutputOperations);
        case 14: return Instruction(Instruction::Type::Multiply, 1, MultiplyOperations);
        case 15: return Instruction(Instruction::Type::Execute, 1, ExecuteOperations);
        case 16: return Instruction(Instruction::Type::Negate, 1, NegateOperations);
        case 17: return Instruction(Instruction::Type::Pop, 1, PopOperations);
        case 18: return Instruction(Instruction::Type::Rbrace, 1, RbraceOperations);
        default: return Instruction(Instruction::Type::Halt, 1, HaltOperations);
    }
}

#endif  // INSTRUCTION_HPP
