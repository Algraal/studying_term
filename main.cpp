#include <iostream>
#include <string>

#include "commands.h"
#include "row_input_handle.h"

// finds matching service brackets (content in brackets has the highest 
// priority). Returns brackets substring, removing it from the source 
// string. If brackets substring is not found returns empty string
std::string split_brackets(std::string &src)
{
   std::string::size_type start_pos = find_symbol_position(src, "(");
   std::string::size_type end_pos = reverse_find_symbol_position(src, ")");
   // if one of the positions npos - error, if start > end - error
   // that kind of errors should be parced by raw input analysator
   // these checks are not required.
   if((start_pos != std::string::npos && end_pos != std::string::npos 
               && start_pos < end_pos))
   {
        // Brackets are deleted in src and results
       std::string res = pop_substr(src, start_pos, end_pos);
       // copies substring, deleting the first and the last symbols in the res
       // string. 1 is symbol '(', 2 symbols is ')'.
       return res.substr(1, res.length() - 2);
   }
   else
   {
       return "";
   }
}

Commands split_delimiters(std::string &src) 
{
    // block that finds the first occurance outside quotes of every delimiter
    std::string::size_type and_pos = std::string::npos;
    std::string::size_type or_pos = 
        find_symbol_position(src, Delimiters::OR);
    std::string::size_type bg_mode_pos = 
        find_symbol_position(src, Delimiters::AMPERSAND);
    std::string::size_type sequently_pos = 
        find_symbol_position(src, Delimiters::SEMICOLON);
    // special case for && and &, the simplest way
    if(bg_mode_pos != std::string::npos)
    {
        // if the first symbol is & and the next one is & we get &&
        if(is_char_on_pos(src, bg_mode_pos + 1, '&')) {
            and_pos = bg_mode_pos;
            bg_mode_pos = std::string::npos;
        }
    }
    // map for storing delimiter and its position
    std::map<std::string, std::string::size_type> del_positions;
    // sets postions of first occurance of Delimiters
    del_positions.insert(std::make_pair(Delimiters::AND, and_pos));
    del_positions.insert(std::make_pair(Delimiters::OR, or_pos));
    del_positions.insert(std::make_pair(Delimiters::AMPERSAND, bg_mode_pos));
    del_positions.insert(std::make_pair(Delimiters::SEMICOLON, sequently_pos));
    // seeks for first delimiter in a provided string 
    std::string initial_key= "";
    std::pair<std::string, std::string::size_type> minimum_value = 
        std::make_pair(initial_key, std::string::npos);
    for(const std::pair<std::string, std::string::size_type> &d_pos : 
            del_positions)
    {
        if(d_pos.second < minimum_value.second)
        {
            minimum_value = std::make_pair(d_pos.first, d_pos.second);
        }
    }
    std::string result;
    if(minimum_value.second != std::string::npos) {
        // Pops out a substring from the beggining till the first
        // occurence of the delimiter. first.length() is used because 
        // delimitres can be different sizes
        result = pop_substr(src, 0, 
                minimum_value.second + minimum_value.first.length() - 1);
        // deletes delimiter from the substrin
        result = result.substr(0, 
                result.length() - minimum_value.first.length());
    }
    else
    {
        // No delimitres case. Pops out the whole src to result
        result = pop_substr(src, 0, src.length() - 1); 
    }
    Commands command_instance(minimum_value.first);
    command_instance.string_to_tokens(result);
    return command_instance;
}

// splits user input into the simplest sets of commands
// !!!!!
// Commands is replaced for debug
// recursivly disassembles input, initializes new Command objects
void input_to_commands(std::vector<Commands> &coms_vec, 
        std::string &src)
{

    if(!src.empty())
    {
        std::string del = "";
        coms_vec.push_back(split_delimiters(src));
        input_to_commands(coms_vec, src);
    }
}

int main()
{
    std::string input;
    std::vector<Commands> commands_list;
// testing spliting input into commands
    getline(std::cin, input);
/* Print test 
    while(input != "exit")
    {
        
        input_to_commands(commands_list, input);
        for(Commands &c : commands_list)
        {
            c.print_tokens();
        }
        commands_list.clear();
        getline(std::cin, input);
    }
*/

	while(input != "exit")
    {
        
        input_to_commands(commands_list, input);
        for(Commands &c : commands_list)
        {
            c.execute_commands();
        }
        commands_list.clear();
        getline(std::cin, input);
    }
    return 0;
}
