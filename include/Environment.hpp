/// ============================
/// @author Andrei Biu - Pislaru
/// @brief Implementation of Environment "static" class
/// ============================

#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <cstdlib>
#include <iostream>
#include "Typedefs.hpp"

/**
 * @class Environment
 * @brief Contains data & functions that represents
 *        the execution environment of the interpreter
 *        including program arguments
 */
class Environment
{
    /// @defgroup Static Fields
    private:
        static byte base;
        static const char* programFilename;

    /// @defgroup Static Getters & Setters
    public:
        static byte Base();
        static const char* ProgramFilename();

    /// @defgroup Static Methods
    public:
        static void Initialize(int argCount, char** argValues);
        static void ExitNormal();
        static void ExitError(xword instructionIndex);
        static void ExitException(xword instructionIndex);
};


/// @ingroup Static Methods
/// @{
inline byte Environment::base;
inline const char* Environment::programFilename;
/// @}

/// @defgroup Static Getters & Setters
/// @{
INLINE byte Environment::Base() { return base; }
INLINE const char* Environment::ProgramFilename() { return programFilename; }
/// @}

/// @ingroup Static Methods
/// @{

INLINE void Environment::Initialize(int argCount, char** argValues)
{
    if (argCount < 2 || argCount > 3)
    {
        std::cerr << "Invalid number of arguments." << std::endl;
        exit(-100);
    }
    programFilename = argValues[1];
    base = argCount == 2 ? 10 : std::stoi(argValues[2]);
    if (base < 2 || base > 37)
    {
        std::cerr << "Invalid base." << std::endl;
        exit(-100);
    } 
}

INLINE void Environment::ExitNormal()
{
    exit(0);
}

INLINE void Environment::ExitError(xword instructionIndex)
{
    std::cerr << "Error:" << instructionIndex << std::endl;
    exit(-1);
}

INLINE void Environment::ExitException(xword instructionIndex)
{
    std::cerr << "Exception:" << instructionIndex << std::endl;
    exit(-2);
}

/// @}

#endif  // ENVIRONMENT_HPP
