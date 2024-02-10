#include <iostream>
#include <algorithm>
#include "commands.h"

bool Commands::string_to_tockens(const std::string &src)
{
    // variables-modifiers of input
    bool is_quoted = false;
    bool is_shilded = false;
    bool is_previous_sep = false;
    std::string tocken;
    for(const char &ch : src)
    {
        if(ch == '"' && !is_shilded)
        {
            if(is_quoted)
            {
                // text inside double quotes is considered a tocken
                tockens.push_back(tocken);
                tocken = "";
                is_quoted = false;
            } 
            else
            {
                is_quoted = true;
            }
        }
        else if(is_shilded)
        {
            if(ch == '"')
            {
                tocken += '\"';  
            }
            else if(ch == '\\')
            {
                tocken += '\\';
            }
        }
        else if((ch == ' ' || ch == '\t') && !is_quoted && !is_previous_sep)
        {
            if(!tocken.empty())
            {
                // pushes tocken after separator
                tockens.push_back(tocken);
                tocken = "";
            }
        }
        else if((ch == '\\' && !is_shilded) || ((ch == '\t' || ch == ' ') && is_previous_sep))
        {
            // these cases are handled separetly because they should be used
            // during next iteration
        }
        else
        {
            tocken += ch;
        }
        // next char will be shilded after '\'
        if(ch == '\\' && !is_shilded)
        {
            is_shilded = true;
        }
        else if(is_shilded)
        {
            is_shilded = false;
        }
        is_previous_sep = false;
        if((ch == ' ' || ch == '\t') && !is_quoted)
        {
            is_previous_sep = true;
        }
    }
    // pushes last tocken
    if(!tocken.empty())
    {
        tockens.push_back(tocken);
    }
    if(is_quoted)
    {
        tockens.clear();
        return false;
    }
    return true;
}

void Commands::print_tockens() const
{
    for(const std::string &tocken : tockens)
    {
        std::cout << '[' << tocken << "] ";
    }
    std::cout << std::endl;
}
