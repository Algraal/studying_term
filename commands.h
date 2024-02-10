#ifndef COMMANDS_H_SENTRY
#define COMMANDS_H_SENTRY
#include <vector>
#include <string>

class Commands
{
public:
    // two constructors are necessary without any (for first set of commands)
    // and with parametres to store modifiers
    Commands();

private:
    // tockens saves a set of commands, flags, filenames and other tockens
    // to be executed by execve
    std::vector<std::string> tockens;
    bool string_to_tockens(const std::string &src);
};

#endif
