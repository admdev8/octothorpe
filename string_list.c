#include "string_list.h"
#include "dmalloc.h"

void strlist_insert_at_begin(strlist_node **l, char *s)
{
    strlist_node *n;
    
    if (*l==NULL)
    {
        // allocate initial
        *l=DCALLOC (strlist_node, 1, "strlist_node");
    };

    n=DCALLOC (strlist_node, 1, "strlist_node");
    n->s=(*l)->s;
    n->next=(*l)->next;

    (*l)->next=n;
    (*l)->s=s;
};

char* strlist_get_first(strlist_node *l)
{
    return l->s;
};

void strlist_free (strlist_node* l, void (*free_fn)(void*))
{
    if (l->next)
        strlist_free (l->next, free_fn);

    if (free_fn)
        (*free_fn)(l->s);
    DFREE (l);
};

