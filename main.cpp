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
            quotes_positions.insert(std::make_pair(opening_quote, 
                        std::string::npos));
        }
    return quotes_positions;
}
// function to check if passed position inside one of the ranges provided
// in map of indexes
bool is_pos_quoted(const std::map<std::string::size_type, std::string::size_type> 
        quotes_map, std::string::size_type pos)
{
    for(const std::pair<const std::string::size_type, std::string::size_type> 
            &range_points : quotes_map)
    {
        if(range_points.first > pos)
        {
            return false;
        }
        // first property is the begining of the range, second is the end
        if(range_points.first < pos && pos < range_points.second)
        {
            return true;
        }
    }
    return false;
}
// splits user input into the simplest sets of commands
// !!!!!
// Commands::Commands is replaced for debug
// recursivly disassembles input, initializes new Command objects
bool input_to_commands(
        std::map<std::string::size_type, std::string::size_type> quotes_map,
        std::string &src)
{
    std::string priority_commands;
    std::string::size_type start_pos = 0;
    std::string::size_type end_pos = 0;

    while(start_pos != std::string::npos && start_pos <= src.length()-1)
    {
        start_pos = src.find('(', start_pos);        
        // This segment looks for code code in (  )
        if(!is_pos_quoted(quotes_map, start_pos))
        {
            end_pos = src.find(')', end_pos);
            if(end_pos != std::string::npos && 
                !is_pos_quoted(quotes_map, end_pos))
            {
                priority_commands = src.substr(start_pos, end_pos - start_pos);
                std::string tmp_sliced_str = src.substr(0, start_pos);
                if(end_pos != src.length() - 1)
                {
                    tmp_sliced_str += src.substr(end_pos + 1, src.length() - end_pos - 1);
                }
                // part of row input is deleted
                src = tmp_sliced_str;
            }
        }
        if(start_pos != std::string::npos && start_pos != src.length()-1)
        {
            start_pos++;
        }
        if(end_pos != std::string::npos && end_pos != src.length() - 1)
        {
            end_pos++;
        }
    }
    return false;   
}
std::vector<std::string> split_row_input(std::string &src)
{
    std::map<std::string::size_type, std::string::size_type> quotes_positions 
                = find_matched_quotes(src);
    input_to_commands(quotes_positions, src);
    std::cout << src << std::endl;
    std::vector<std::string> d;
    return d;
}

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
        split_row_input(input);
        getline(std::cin, input);
    }
    return 0;
}
