#ifndef TERMINAL_TOKENS_H
#define TERMINAL_TOKENS_H

enum { max_token_size = 4096 };

// structure to store commands, flags, file names e.t.c.
// is an item of list
struct word_token 
{
    char *word;
    struct word_token *next;
};
// structure to store list of tokens, enviroment of comands, group, fd, e.t.c.
struct command_set
{
    struct word_token *first;
    struct word_token *last;
};

// function for debug
void wt_print_list(struct word_token *first);
// allocates memory defined by size and copies 
char *create_str_nulltr(const char *src, int size);
// function creates word_token struct and puts it into passed 
// struct command_set to property last (if list is empty puts
// to property first and to property last)
void create_item(struct command_set *command_env, 
        const char *src, int size);
// frees allocated memory for list of word_tokens starting
// from first element
void wt_clear_list(struct word_token *first);
// creates a list of words contained in a provided string.
// returns pointer of type struct word_token to the first element of the
// list. If string was empty points to NULL
int string_to_list(const char *string_input, 
        struct command_set *command_env);

#endif
