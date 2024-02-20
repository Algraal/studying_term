#ifndef COMMANDS_H_SENTRY
#define COMMANDS_H_SENTRY
#include <vector>
#include <string>

namespace Delimiters {
    constexpr char AND[] = "&&";
    constexpr char OR[] = "||";
    constexpr char SEMICOLON[] = ";";
    constexpr char AMPERSAND[] = "&";
}

class Commands
{
public:
    // two constructors are necessary without any (for first set of commands)
    // and with parametres to store modifiers
    Commands(std::string delimiter = "") : end_delimiter(delimiter) {};
    bool string_to_tokens(const std::string &src);
    // prints tokens for debug
    void print_tokens() const;
    // executes request using tokens as a command for execvp
    bool execute_commands();
private:
    // tokens stores a set of commands, flags, filenames and other tokens
    // to be executed by execve
    std::vector<std::string> tokens;
    // property that defines delimiter that separates from another command
    std::string end_delimiter;
    // function to execute terminal cd commad
};

#endif
