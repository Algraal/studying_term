#ifndef COMMANDS_H_SENTRY
#define COMMANDS_H_SENTRY
#include <vector>
#include <string>

namespace Delimiters {
    constexpr char AND[] = "&&";
    constexpr char OR[] = "||";
    constexpr char SEMICOLON[] = ";";
    constexpr char AMPERSAND[] = "&";
    constexpr char WRITE[] = ">";
    constexpr char APPEND[] = ">>";
    constexpr char READ[] = "<";
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
    inline std::string get_end_delimiter() { return end_delimiter; };
    // executes request using tokens as a command for execvp
    bool execute_commands();
private:
    // tokens stores a set of commands, flags, filenames and other tokens
    // to be executed by execve
    std::vector<std::string> tokens;
    // property that defines delimiter that separates from another command
    void set_redirection();
    std::string end_delimiter;
    std::string file_name;
    std::string redirection_type;
    // function to execute terminal cd commad
};

#endif
