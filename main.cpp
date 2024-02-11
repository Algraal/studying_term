#include <iostream>
#include <string>
#include <forward_list>
#include <map>

#include "commands.h"


// checks if next symbol is a passed one
bool is_next_char(std::string src, std::string::size_type pos, char next_ch)
{
    if(pos + 1 < src.length())
    {
        if(src.at(pos+1) == next_ch)
        {
            return true;
        }
    }
    return false;
}

std::map<std::string::size_type, std::string::size_type> find_matched_quotes(
        const std::string &src)
{
    std::map<std::string::size_type, std::string::size_type> quotes_positions;
    bool is_shielded = false;
    bool is_quoted = false;
    std::string::size_type opening_quote = std::string::npos;
    for(std::string::size_type pos = 0; pos < src.length(); ++pos)
    {
        const char &ch = src[pos];
        if(ch == '"' && !is_shielded)
        {
            if(!is_quoted)
            {
                is_quoted = true;
                opening_quote = pos;
            }
            else
            {
                is_quoted = false;
                quotes_positions.insert(std::make_pair(opening_quote, pos)); 
                opening_quote = std::string::npos;
            }
        }
        // unmatched quote case is not considered an error in splitting input.
        // this error is handled later (handling it now can break the order of execution)
        if(ch == '\\' && !is_shielded)
        {
            is_shielded = true; 
        }
        else if(is_shielded)
        {
            is_shielded = false;
        }
    }
     if(is_quoted)
        {
            quotes_positions.insert(std::make_pair(opening_quote, std::string::npos));
        }
    return quotes_positions;
}

/*
std::vector<std::string> split_row_input(std::string src)
{
    std::map<std::string::size_type, std::string::size_type> quotes_positions 
                = find_matched_quotes(src);
}
*/
int main()
{
    std::string input;
    std::forward_list<Commands> commands_list;
// testing split string into tokens
    getline(std::cin, input);
/*    
    while(input != "exit")
    {
        Commands com;
        com.string_to_tokens(input);
        com.print_tokens();
        com.execute_commands();
        getline(std::cin, input);
    }
*/
// Testing quotes
    while(input != "exit")
    {
        std::map<std::string::size_type, std::string::size_type> quotes_positions
            = find_matched_quotes(input);
        for(const std::pair<const std::string::size_type, std::string::size_type>
                &pos_pair : quotes_positions)
        {
            std::cout << pos_pair.first << " " << pos_pair.second << std::endl;
        }
        getline(std::cin, input);
    }
    return 0;
}
