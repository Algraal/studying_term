#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <array>

#include "commands.h"

bool Commands::string_to_tokens(const std::string &src)
{
    // variables-modifiers of input
    bool is_quoted = false;
    bool is_shilded = false;
    bool is_previous_sep = false;
    std::string del;
    std::string token;
    for(const char &ch : src)
    { 
        if(ch == '"' && !is_shilded)
        {
            if(is_quoted)
            {
                // text inside double quotes is considered a token
                tokens.push_back(token);
            } 
            else
            {
                // empty token can be entered only using ""
                if(!token.empty())
                {
                tokens.push_back(token);
                }
            }
            token = "";
            is_quoted ^= true;
        } 
        else if(is_shilded)
        {
            // other symbols after shileding symbols are skipped
            switch(ch)
            {
                case '"': token += '"'; break;
                case '\\': token += '\\'; break;
                // can be implemented later
            } 
        }
        else if((ch == ' ' || ch == '\t') && !is_quoted && !is_previous_sep)
        {
            if(!token.empty())
            {
                // pushes token after separator
                tokens.push_back(token);
                token = "";
            }
        }
        else if((ch == '\\' && !is_shilded) || ((ch == '\t' || ch == ' ') && is_previous_sep))
        {
            // these cases are handled separetly because they should be used
            // during next iteration
        }
        else
        {
            token += ch;
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
    // pushes last token
    if(!token.empty())
    {
        tokens.push_back(token);
    }
    // if string contained unmatched quote it is impossible to split it into 
    // tokens
    if(is_quoted)
    {
        std::cout << "Error: unmatched quotes" << std::endl;
        return false;
    }
    return true;
}

bool Commands::execute_commands() const
{
    if(tokens.empty())
    {
        perror("Empty tokens");
        return false;
    }
    if(tokens.front() == "cd")
    {
        return this->execute_change_of_directory(); 
    }
    // flushes buffer to prevent duplication of data in processes
    std::vector <char*> cstyle_tokens;
    // copies content of tocken vector of strings into vector of 
    // c style strings
    for(const std::string& token : tokens)
    {
        cstyle_tokens.push_back(const_cast<char*>(token.c_str()));
    }
    // adds null_ptr to cstring. execvp expects NULL at the end of
    // array of commands
    cstyle_tokens.push_back(nullptr);
    
    std::cout << std::flush;
    int pid = fork();
    if(pid == -1)
    {
        perror("pid");
        return false;
    }
    if(pid == 0)
    {
        execvp(cstyle_tokens.at(0), cstyle_tokens.data());
        perror("execvp");
        return false;
    }
    wait(NULL);
    return true;
}

void Commands::print_tokens() const
{
    for(const std::string &token : tokens)
    {
        std::cout << '[' << token << "] ";
    }
    std::cout << "DEL: '" << end_delimiter << "'" << std::endl;
    std::cout << std::endl;
}

bool Commands::execute_change_of_directory() const
{
    if(tokens.size() == 1)
    {
        // secure_genev another option, but I am not sure how effecient it for the terminal app
        const char* path = std::getenv("HOME");
        if(path != nullptr)
        {
            if(!chdir(path))
            {
                return true;
            }
            else
            {
                perror("chdir");
                return false;
            }
        }
        else
        {
            perror("Enviroment");
            return false;
        }
    }
    else if(tokens.size() == 2)
    {
        // string to c_str
        if(!chdir(tokens.at(1).c_str()))
        {
            return true;
        }
        else
        {
            perror("Invalid path");
            return false;
        }
    }
    else
    {
        // this terminal does not provide any flags for cd command
        return false;
    }
}
