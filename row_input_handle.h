#ifndef ROW_INPUT_HANDLE_H
#define ROW_INPUT_HANDLE_H

#include <iostream>
#include <string>
#include <map>

// Checks if symbol at a passed position is a passed one. Cheks if positions is
// out of bounds
bool is_char_on_pos(const std::string &src, std::string::size_type pos, 
        char next_ch);
// returns a map with ranges of quotes
std::map<std::string::size_type, std::string::size_type> find_matched_quotes(
        const std::string &src);
// function to check if passed position inside one of the ranges provided
// in map of indexes
bool is_pos_quoted(const std::string &src, std::string::size_type pos);
// returns position of the last occurance of req_str outside quotes
std::string::size_type reverse_find_symbol_position(const std::string &src, 
        std::string req_str);
// returns position of the first occurance of req_str outside quotes
std::string::size_type find_symbol_position(const std::string &src, 
        std::string req_str);
// Popes a substring out of a passed string, according to given positions 
// Changes src string. Returns poped out string
std::string pop_substr(std::string &src, std::string::size_type start_pos, 
        std::string::size_type end_pos);

#endif
