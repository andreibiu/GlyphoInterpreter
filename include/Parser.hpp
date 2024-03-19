/// ============================
/// @author Andrei Biu - Pislaru
/// @brief Implementation of Parser "static" class
/// ============================

#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <vector>
#include <stack>
#include <string>

#include "Instruction.hpp"

/**
 * @namespace Parser
 * @brief Contains functions that transform Glypho programs into arrays of
 *        instructions
 */
namespace Parser
{
    INLINE std::vector<Instruction> Parse()
    {
        bool hasError = false;
        xword readCounter = 0, instructionIndex = 0;
        char instructionCodeSymbols[4];
        std::vector<Instruction> instructions;
        std::stack<xword> lbraceIndices;
        std::ifstream stream(Environment::ProgramFilename());

        while (stream.peek() != EOF)
        {
            // Read the symbolic code of the next instruction
            readCounter += 1;
            stream >> instructionCodeSymbols[0]; 
            readCounter += stream.peek() != EOF;
            stream >> instructionCodeSymbols[1];
            readCounter += stream.peek() != EOF;
            stream >> instructionCodeSymbols[2];
            readCounter += stream.peek() != EOF;
            stream >> instructionCodeSymbols[3];

            // Check for insufficient symbols for the last instructions
            // This kind of error has the highest priority
            if (readCounter & 3)
            {
                stream.close();
                Environment::ExitError(readCounter >> 2);
            }

            // Only keep reading for invalid file length if other type of
            // error has been detected
            if (hasError)
                continue;

            // Save the instruction and informations about brace instructions
            // Check if a rbrace appears before a lbrace
            instructions.push_back(Instruction::GetNext(instructionCodeSymbols));
            if (instructions.back().Type() == Instruction::Type::Lbrace)
            {
                lbraceIndices.push(instructionIndex);
            }
            if (instructions.back().Type() == Instruction::Type::Rbrace)
            {
                if (lbraceIndices.empty())
                {
                    hasError = true;
                    continue;
                }

                long offset = instructionIndex - lbraceIndices.top();
                instructions.back().Offset(-offset);
                instructions[lbraceIndices.top()].Offset(offset + 1);
                lbraceIndices.pop();
            }
            ++instructionIndex;
        }

        stream.close();

        // Final checks for brace errors 
        if (hasError)
            Environment::ExitError(instructionIndex);
        if (!lbraceIndices.empty())
            Environment::ExitError(readCounter >> 2);

        return instructions;
    }
};

#endif  // PARSER_HPP
