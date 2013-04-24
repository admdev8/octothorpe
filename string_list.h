#pragma once

typedef struct _strlist_node
{
    char* s;
    struct _strlist_node *next;
} strlist_node;

#ifdef  __cplusplus
extern "C" {
#endif

void strlist_insert_at_begin(strlist_node **l, char *s);
char* strlist_get_first(strlist_node *l);
void strlist_free (strlist_node* l, void (*free_fn)(void*));

#ifdef  __cplusplus
}
#endif

