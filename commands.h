#ifndef COMMANDS_H_SENTRY
#define COMMANDS_H_SENTRY
#include <vector>
#include <string>

class Commands
{
public:
    // two constructors are necessary without any (for first set of commands)
    // and with parametres to store modifiers
    Commands(bool bg_mode = false) : and_state(false), or_state(false), 
        bg_mode_state(false), sequently_state(false) {};
    bool string_to_tokens(const std::string &src);
    // prints tokens for debug
    void print_tokens() const;
    // executes request using tokens as a command for execvp
    bool execute_commands() const;
    // functions to change states. Mb will switch to bitfield
    inline void set_and_state(bool state) { and_state = state; };
    inline void set_or_state(bool state) { or_state = state; };
    inline void set_bg_mode_state(bool state) { bg_mode_state = state; };
    inline void set_sequently_state(bool state) { sequently_state = state; };
    // functions to get value of bool flag
    inline bool get_and_state() { return and_state; };
    inline bool get_or_state() { return or_state; };   
    inline bool get_bg_mode_state() { return bg_mode_state; };
    inline bool get_sequently_state() { return sequently_state; };
    void is_next_char();

private:
    // tokens stores a set of commands, flags, filenames and other tokens
    // to be executed by execve
    std::vector<std::string> tokens;
    // mb change to bit field
    bool and_state;
    bool or_state;
    bool bg_mode_state;
    bool sequently_state; 
};

#endif
