#include <iostream>
#include <string>
#include "commands.h"

int main()
{
    std::string input;
    getline(std::cin, input);
    while(input != "exit")
    {
        Commands com;
        com.string_to_tockens(input);
        com.print_tockens();
        getline(std::cin, input);
    }
    return 0;
}
