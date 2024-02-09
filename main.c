#include "terminal_tokens.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct command_set *set = malloc(sizeof(struct command_set));
    set->first = set->last = NULL;
    string_to_list(argv[1], set);
    
    wt_print_list(set->first);
    wt_clear_list(set->first);
    free(set);
    return 0;
}
