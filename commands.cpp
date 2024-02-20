#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <vector>

#include "commands.h"


static bool execute_and_wait(const std::vector<std::string> &tokens);
static bool bg_mode_execute(const std::vector<std::string> &tokens);
static std::vector<char*> vector_to_pointer_vector(const std::vector<std::string> 
        &tokens);
static bool execute_change_of_directory(const std::vector<std::string> &tokens);


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

bool Commands::execute_commands() 
{
    if(tokens.empty())
    {
        perror("Empty tokens");
        return false;
    }
    if(get_end_delimiter() == Delimiters::AMPERSAND)
    {
        return bg_mode_execute(tokens);
    }
    else
    {
        return execute_and_wait(tokens);
    }
    // !!! CHANGE cd can be used with delimitres, it should be considered

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

static bool execute_change_of_directory(const std::vector<std::string> &tokens)
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

// executes provided set of tokens using execve, waits until it finishes,
// returns true if finished successfully, otherwise false
static bool execute_and_wait(const std::vector<std::string> &tokens)
{
    if(tokens.front() == "cd")
    {
        return execute_change_of_directory(tokens); 
    }
    std::vector <char*> cstyle_tokens = vector_to_pointer_vector(tokens);
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
    // Proper waitpid handling to ensure that next process will be handled
    // correctly even with delimiter. 
    int status;
    pid_t p;
    do
    {
        p = waitpid(pid, &status, 0);
        if(p == -1)
        {
            perror("waitpid");
            return false;
        }
    }
    // WIFEXITED returns true if the child terminated nomrally
    // WIFSIGNALED returns true if the child was terminated by a signal
    while(!WIFEXITED(status) && !WIFSIGNALED(status));
    
    if(WIFEXITED(status))
    {
        int exit_status = WEXITSTATUS(status);
        if(exit_status == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    // I tested killing process with SIGKILL, work of process was 
    // considered successful.
    else if (WIFSIGNALED(status))
    {
        return true;
    }
    // I am not sure if this case is even possible
    return false;
}

static bool bg_mode_execute(const std::vector<std::string> &tokens)
{
    if(tokens.front() == "cd")
    {
        return execute_change_of_directory(tokens); 
    }
    std::vector<char*> cstyle_tokens = vector_to_pointer_vector(tokens);
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
    return true;
}

static std::vector<char*> vector_to_pointer_vector(const std::vector<std::string> 
        &tokens)
{
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
    return cstyle_tokens;
}

