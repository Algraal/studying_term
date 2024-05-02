#include "../include/row_input_handle.h"


bool is_char_on_pos(const std::string &src, std::string::size_type pos, char next_ch)
{
    if(pos >= 0 && pos < src.length())
    {
        if(src.at(pos) == next_ch)
        {
            return true;
        }
    }
    return false;
}

static std::map<std::string::size_type, std::string::size_type> find_matched_quotes(
        const std::string &src)
{
    std::map<std::string::size_type, std::string::size_type> quotes_positions;
    bool is_quoted = false;
    std::string::size_type opening_quote = std::string::npos;
    for(std::string::size_type pos = 0; pos < src.length(); ++pos)
    {
        const char &ch = src[pos];
        if(ch == '"' && !is_char_on_pos(src, pos - 1, '\\'))
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
    }
    // in case of unmatched quote in the end it cannot be inserted inside loop,
    // so it is done aftwerwards
    if(is_quoted)
    {
        quotes_positions.insert(std::make_pair(opening_quote, 
                    std::string::npos));
    }
    return quotes_positions;
}
// function to check if passed position inside one of the ranges provided
// in map of indexes
bool is_pos_quoted(const std::string &src, std::string::size_type pos)
{
    std::map<std::string::size_type, std::string::size_type> quotes_map = 
        find_matched_quotes(src);
    
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

// returns position of the last occurance of req_str outside quotes
std::string::size_type reverse_find_symbol_position(const std::string &src, std::string req_str)
{
    std::string::size_type start_pos = src.rfind(req_str);
    while(start_pos != std::string::npos)
    {
        if(!is_pos_quoted(src, start_pos))
        {
            // unquoted bracket is found
            break;
        }
        else
        {
            if(start_pos != 1)
            {
                start_pos--;
            }
            else
            {
                // if there is no unquoted bracket
                start_pos = std::string::npos;
                break;
            }
        }
        start_pos = src.rfind(req_str, start_pos);
    }
    return start_pos;
}


// returns position of the first occurance of req_str outside quotes
std::string::size_type find_symbol_position(const std::string &src, std::string req_str)
{
    std::string::size_type start_pos = src.find(req_str);
    while(start_pos != std::string::npos)
    {
        if(!is_pos_quoted(src, start_pos))
        {
            // unquoted bracket is found
            break;
        }
        else
        {
            if(start_pos != src.length() - 1)
            {
                start_pos++;
            }
            else
            {
                // if there is no unquoted bracket
                start_pos = std::string::npos;
                break;
            }
        }
        start_pos = src.find(req_str, start_pos);
    }
    return start_pos;
}

// Popes a substring out of a passed string, according to given positions 
// Changes src string. Returns poped out string
std::string pop_substr(std::string &src, std::string::size_type start_pos, 
        std::string::size_type end_pos)
{
    // Brackets are deleted in src and results
    std::string result = src.substr(start_pos, end_pos - start_pos + 1);
    // slices of src excluding text in brackets are stored together
    std::string new_src = src.substr(0, start_pos);
    new_src += src.substr(end_pos + 1, src.length() - end_pos - 1);
    // brackets and text inside them are deleted
    src = new_src;
    return result;
}
