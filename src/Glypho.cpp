/// ============================
/// @author Andrei Biu - Pislaru
/// @brief Entry point of the program
/// ============================

#include "../include/Environment.hpp"
#include "../include/Parser.hpp"
#include "../include/Program.hpp"

int main(int argCount, char** argValues)
{
    Environment::Initialize(argCount, argValues);
    Program(Parser::Parse()).Run();
}
