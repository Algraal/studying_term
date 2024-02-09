#include <stdlib.h>
#include <stdio.h>
#include "terminal_tokens.h"

char * create_str_nulltr(const char *src, int size)
{
    char *str = malloc((size+1) * sizeof(char));
    for(int i = 0; i < size; i++)
    {
        str[i] = src[i];
    }
    str[size] = 0;
    return str;
}

void wt_print_list(struct word_token *first) {
    
    int counter = 0;
    struct word_token *ptr = first;
    if(first == NULL)
    {
        return;
    }
    printf("%d - %s\n", counter, ptr->word);
}

void create_item(struct command_set *command_env, 
        const char *src, int size)
{
    struct word_token *tmp = malloc(sizeof(struct word_token));
    tmp->next = NULL;
    tmp->word = create_str_nulltr(src, size);
    printf("%s\nhi\n", tmp->word);
    if(command_env->last)
    {
        command_env->last->next = tmp;
        command_env->last = command_env->last->next;
    }
    else
    {
        command_env->first = command_env->last = tmp;
    }
}
void wt_clear_list(struct word_token *first)
{
    struct word_token *tmp = first;
    struct word_token *prev;
    while(tmp->next)
    {
        free(tmp->word);
        prev = tmp;
        tmp = tmp->next;
        free(prev);
    }
    free(tmp->word);
    free(tmp);
}
int string_to_list(const char *string_input, 
        struct command_set *command_env) 
{
   char token[max_token_size];
   // in the quotes '\t', ' ' symbols do not work as separators
   int is_inside_quotes = 0;
   int token_size = 0;
   const char *string_ptr = string_input;
   while(*string_ptr)
   {
       // error, too long input
       if(token_size >= max_token_size)
       {
           return 1;
       }
       if(*string_ptr == '"')
       {
           // changes state of var: truth to false or false to truth
           is_inside_quotes ^= 1;
       }
       else if((*string_ptr == ' ' || *string_ptr == '\t') && !is_inside_quotes)
       {
           create_item(command_env, token, token_size);
           // token buff is empty
           token_size = 0;
       }
       else
       {
           token[token_size] = *string_ptr;
           token_size++;
       }
       string_ptr++;
   }
   return 0;
}
