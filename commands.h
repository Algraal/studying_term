#ifndef COMMANDS_H_SENTRY
#define COMMANDS_H_SENTRY
#include <vector>
#include <string>

class Commands
{
public:
    // two constructors are necessary without any (for first set of commands)
    // and with parametres to store modifiers
    //Commands();
    bool string_to_tokens(const std::string &src);
    // prints tokens for debug
    void print_tokens() const;

private:
    // tokens stores a set of commands, flags, filenames and other tokens
    // to be executed by execve
    std::vector<std::string> tokens;
};

#endif
