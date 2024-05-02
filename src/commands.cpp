#include <cstdlib>
#include <iostream>
#include <ostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <vector>
#include <fcntl.h>

#include "../include/commands.h"
// provides function is_pos_quoted, is_char_on_pos
#include "../include/row_input_handle.h"

static bool execute_and_wait(const std::vector<std::string> &tokens);
static bool bg_mode_execute(const std::vector<std::string> &tokens);
static std::vector<char*> vector_to_pointer_vector(const std::vector<std::string> 
        &tokens);
// changes directory, returns PID of a current process on success, otherwise -1
static int execute_change_of_directory(const std::vector<std::string> &tokens);
// executes changes passed in vector of strings using execvp, return PID on
// success, -1 otherwise
static int execute_tokens_return_pid(const std::vector<std::string> &tokens);
// replaces passed std_fd with passed fd, returns copy of std_fd
static int redirect(int std_fd, int file_fd);
static std::string remove_tokens_for_redirection(std::vector<std::string> &tokens, size_t pos);


bool Commands::string_to_tokens(const std::string &src)
{
    std::string token = "";
    size_t pos;
    for(pos = 0; pos < src.length(); ++pos)
    {
        // if char is unshielded quote 
        if(src.at(pos) == '"' && !is_char_on_pos(src, pos - 1, '\\'))
        {
            // empty quotes "" are considered a suitable token
            if(is_pos_quoted(src, pos))
            {
                tokens.push_back(token);
            }
            else
            {
                token.empty() ? void() : tokens.push_back(token);           
            }
            token = "";
        }
        else if(src.at(pos) == '\\' && !is_pos_quoted(src, pos) && 
                !is_char_on_pos(src, pos - 1, '\\'))
        {
            // do nothing
        }
        else if((src.at(pos) == ' ' || src.at(pos) == '\t') && 
                !is_pos_quoted(src, pos) &&
                !is_char_on_pos(src, pos-1, '\\'))
        {
            token.empty() ? void() : tokens.push_back(token);
            token = "";
        }
        else if(src.at(pos) == '<' && !is_pos_quoted(src, pos) && 
                !is_char_on_pos(src, pos - 1, '\\'))
        {
            // pushes current token and "<" redirector (redirectors are 
            // considered an independent tokens for easier processing)
            token.empty() ? void() : tokens.push_back(token);
            tokens.push_back("<");
            token = "";
        }
        else if(src.at(pos) == '>' && !is_pos_quoted(src, pos) && 
                !is_char_on_pos(src, pos - 1, '\\'))
        {
            token.empty() ? void() : tokens.push_back(token);
            token = "";
            if(is_char_on_pos(src, pos + 1, '>'))
            {
                tokens.push_back(">>");
                // we should skip next step if next symbol is also >
                pos++;
            }
            else
            {
                tokens.push_back(">");
            }
        }
        else
        {
            token += src.at(pos);
        }
    }
    // pushes last token if it exists
    token.empty() ? void() : tokens.push_back(token);
    // checks if last character is quoted as it means unmatched quote
    if(is_pos_quoted(src, pos -1))
    {
        std::cout << "Error: unmatched quote." << std::endl;
        return false;
    }
    // redirection analysis
    set_redirection(); 
    return true;
}
/*
bool Commands::string_to_tokens(const std::string &src)
{
    // variables-modifiers of input
    bool is_quoted = false;
    bool is_shilded = false;
    bool is_previous_sep = false;
    bool is_append_ch = false;
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
        // splits redirection symbols into separated tokens for easier handling
        else if((ch == '<' || ch =='>') && !is_shilded && !is_quoted)
        {
            if(ch == '<')
            {
                // if some token exist we push it
                if(!token.empty())
                {
                    tokens.push_back(token);
                    token = "";
                }
                // in any case push read symbol
                tokens.push_back("<");
            }
            else if(ch == '>')
            {
                if(is_append_ch)
                {
                    // pushes token if it exists
                    if(!token.empty())
                    {
                        tokens.push_back(token);
                        token = "";
                    }
                    // anyway pushes append
                    tokens.push_back(">>");
                    is_append_ch = false;               
                }
                else
                {
                    if(!token.empty())
                    {
                        tokens.push_back(token);
                        token = "";
                    }
                    is_append_ch = true;
                }
            }
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
        if(is_append_ch)
        {
            if(ch != '>')
            {
                tokens.push_back(">");
                is_append_ch = false;
            }

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
    // redirection analysis
    set_redirection(); 
    return true;
}
*/
// deletes redirection delimiter and file name if it exists, returns file_name
static std::string remove_tokens_for_redirection(std::vector<std::string> &tokens, size_t pos)
{
    // check if a file for redirection exist
    std::string file_name = ""; 
    if(pos + 1 < tokens.size())
    {
        // checks if next string is not redirection delimiter
        if(tokens.at(pos+1) != Delimiters::WRITE && 
                tokens.at(pos+1) != Delimiters::APPEND &&
                tokens.at(pos+1) != Delimiters::READ)
        {
            std::vector<std::string>::iterator index = tokens.begin() + pos + 1;
            file_name = tokens.at(pos + 1);
            tokens.erase(index);
        }
    }
    tokens.erase(tokens.begin() + pos);
    // returns empty string if there is no filename
    return file_name;
}

void Commands::set_redirection()
{
    // Set of strings to find
    std::vector<std::string> delimiters_vector = {Delimiters::WRITE,Delimiters::APPEND, Delimiters::READ};
    // Iterate through the vector
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if(Delimiters::WRITE == tokens.at(i))
        {
            redirection_type = Delimiters::WRITE;
            file_name = remove_tokens_for_redirection(tokens, i);
        }
        else if(Delimiters::APPEND == tokens.at(i))
        {
            redirection_type = Delimiters::APPEND;
            file_name = remove_tokens_for_redirection(tokens, i);
        }
        else if(Delimiters::READ == tokens.at(i))
        {
            redirection_type = Delimiters::READ;
            file_name = remove_tokens_for_redirection(tokens, i); 
        }
    }
}


bool Commands::execute_commands() 
{
    if(tokens.empty())
    {
        perror("Empty tokens");
        return false;
    }
    // Block of code that handles redirection
    // Standard fd for stdin, stdout, stderr are 0,1,2 respectively
    int fd = -2;
    int save_fd = 0;
    int std_fd = -1;
    if(redirection_type == Delimiters::WRITE)
    {
        fd = open(file_name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if(fd == -1)
        {
            perror("Open");
            return false;
        }
        std_fd = 1;
        save_fd = redirect(std_fd, fd);
    }
    else if(redirection_type == Delimiters::APPEND)
    {
        fd = open(file_name.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666);
        if(fd == -1)
        {
            perror("Open");
            return false;
        }
        std_fd = 1;
        save_fd = redirect(std_fd, fd);
    }
    else if(redirection_type == Delimiters::READ)
    {
        fd = open(file_name.c_str(), O_RDONLY);
        if(fd == -1)
        {
            perror("Open");
            return false;
        }
        std_fd = 0;
        save_fd = redirect(std_fd, fd);
    }
    if(get_end_delimiter() == Delimiters::AMPERSAND)
    {
        bool res = bg_mode_execute(tokens);
        if(fd != -2)
        {
            dup2(save_fd, std_fd);
            close(save_fd);
        }
        return res;
    }
    else
    {
        bool res = execute_and_wait(tokens);
        if(fd != -2)
        {
            dup2(save_fd, std_fd);
            close(save_fd);
        }
        return res;
    }
    return false;
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
static int execute_change_of_directory(const std::vector<std::string> &tokens)
{
    if(tokens.size() == 1)
    {
        // secure_genev another option, but I am not sure how effecient it for the terminal app
        const char* path = std::getenv("HOME");
        if(path != nullptr)
        {
            if(!chdir(path))
            {
                return getpid();
            }
            else
            {
                perror("chdir");
                return -1;
            }
        }
        else
        {
            perror("Enviroment");
            return -1;
        }
    }
    else if(tokens.size() == 2)
    {
        // string to c_str
        if(!chdir(tokens.at(1).c_str()))
        {
            return getpid();
        }
        else
        {
            perror("Invalid path");
            return -1;
        }
    }
    else
    {
        // this terminal does not provide any flags for cd command
        return -1;
    }
}

// executes provided set of tokens using execve, waits until it finishes,
// returns true if finished successfully, otherwise false
static bool execute_and_wait(const std::vector<std::string> &tokens)
{
    // if -1 was returned the process it means procces is not started
    int pid = execute_tokens_return_pid(tokens);
    if(pid == -1)
    {
        return false;
    }
    // case of "cd" is handled separetly, because directoy of a process can
    // be changed only by this process, not with exec call
    else if(pid == getpid())
    {
        return true;
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
// executes process using passed vector of strings as a command 
// returns its PID on success, otherwise returns -1, returns 

static int redirect(int std_fd, int file_fd)
{
    int save1;
    std::cout.flush();
    save1 = dup(std_fd);
    // error handling of file opening is expected to be where file was opened
    // std fd is switched to passed fd
    dup2(file_fd, std_fd);
    close(file_fd);
    return save1;
}

static int execute_tokens_return_pid(const std::vector<std::string> &tokens)
{
    if(tokens.front() == "cd")
    {
        // also returns pid or -1 on failure
        return execute_change_of_directory(tokens); 
    }
    std::vector<char*> cstyle_tokens = vector_to_pointer_vector(tokens);
    std::cout << std::flush;
    int pid = fork();
    if(pid == -1)
    {
        perror("pid");
        return -1;
    }
    if(pid == 0)
    {
        execvp(cstyle_tokens.at(0), cstyle_tokens.data());
        perror("execvp");
        return -1;
    }

    return pid;
}
static bool bg_mode_execute(const std::vector<std::string> &tokens)
{
    int pid = execute_tokens_return_pid(tokens);
    // if background process could not start pid equals -1
    if(pid != -1)
    {
        // May be another output should be implemented
        std::cout << "[" << pid << "]" << std::endl;
        return true;
    }
    return false;
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

