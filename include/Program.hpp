/// ============================
/// @author Andrei Biu - Pislaru
/// @brief Implementation for a Glypho program
/// ============================

#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <vector>
#include "Instruction.hpp"

/**
 * @class Program
 * @brief Models a Glypho program as a sequence of instructions -
 *      stored as an array that can be potentially be
 *      executed in any order - and an index for the current
 *      instruction to be executed  
 */
class Program
{
    /// @defgroup Fields
    private:
        std::vector<Instruction> instructions;
        xword currentInstructionIndex;

    /// @defgroup Constructor
    public:
        Program(std::vector<Instruction> instructions) : instructions(instructions),
                                                         currentInstructionIndex(0) {}

    /// @defgroup Methods
    public:
        // Runs the program
        void Run()
        {
            long offset;
            
            while (currentInstructionIndex < instructions.size())
            {
                try
                {
                    offset = instructions[currentInstructionIndex]();
                    currentInstructionIndex += offset;
                }
                catch (...)
                {
                    Environment::ExitException(currentInstructionIndex);
                }
            }
        }

        // Converts the program to a .glh symbolic representation
        std::string ToString() const
        {
            std::string string;

            for (xword i = 0; i < instructions.size(); ++i)
                string.push_back((char) instructions[i].Type());

            return string;
        }
};

#endif  // PROGRAM_HPP
