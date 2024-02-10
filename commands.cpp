#include <algorithm>
#include "commands.h"

bool Commands::string_to_tockens(const std::string &src)
{
    int is_quoted = 0;
    bool is_shilded = false;
    bool is_previous_sep = false;
    std::string tocken;
    for(const char &ch : src)
    {
        if(ch == '"' && !is_shilded)
        {
            is_quoted ^= 1;
        }
        else if((ch == ' ' || ch == '\t') && !is_quoted && !is_previous_sep)
        {
            tockens.append(tocken);
            tocken = "";
        }
        else if(ch == '\' || ((ch == '\t' || ch == ' ') && is_previous_sep))
        {
            continue;
        }
        else
        {
            tocken += ch;
        }
        // next char will be shilded after '\'
        is_shilded = false; 
        if(ch == '\')
        {
            is_shilded = true;
        }
        is_previous_sep = false;
        if(ch == ' ' || ch == '\t')
        {
            is_previous_sep = true;
        }
    }
    tockens.append(tocken);
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
        std::cout << tocken << " ";
    }
    std::cout << std::endl;
}
