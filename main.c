#include <malloc.h>

// structure to store command, flag, file e.t.c.
// is an item of list
struct word_tocken 
{
    char *word;
    struct word_tocken *next;
};
// creates a list of words contained in a provided string.
// returns pointer of type struct word_tocken to the first element of the
// list. If string was empty points to NULL
struct word_tocken *string_to_list(const char *string_input) 
{
   struct word_tocken *first = NULL; 
   struct word_tocken *last = NULL;
   struct word_tocken *tmp;
   const char *string_ptr = string_input;
   while(*string_ptr)
   {
       tmp 
       string_ptr++;
   }
}

