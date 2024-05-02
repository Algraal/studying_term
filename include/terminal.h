#ifndef TERMINAL_H_SENTRY
#define TERMINAL_H_SENTRY

#include "group.h"
#include "commands.h"

#include <string>
#include <vector>


// Class Terminal handles user input, order of execution of commands (not 
// dicrectly but via orginizing data structures that stores groups of commands).
// Terminal is resposibly for main execution loop.
class Terminal
{
public:

    // finds matching service brackets (content in brackets has the highest 
    // priority). Returns brackets substring, removing it from the source 
    // string. If brackets substring is not found returns empty string
    std::string split_brackets();
    void split_delimiters(std::string &src);
    void input_to_commands();
    int run_terminal();
private:
    std::string terminal_buffer;
    std::vector<Commands> commands_to_execute;
};

#endif
