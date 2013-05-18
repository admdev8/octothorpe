#include "dlist.h"
#include "dmalloc.h"

dlist *dlist_init()
{
    return DCALLOC(dlist, 1, "dlist");
};

void dlist_insert_at_begin(dlist *l, void *data)
{
    assert(l);
    if (l->data==NULL && l->prev==NULL && l->next==NULL)
    {
        l->data=data;
        return;
    };
    dlist *old_head=DCALLOC(dlist, 1, "dlist");
   
    *old_head=*l; // copy whole structure

    // fill current list head
    l->data=data;
    l->next=old_head;
    l->prev=NULL;
};

void* dlist_get_first(dlist *l)
{
    return l->data;
};

void dlist_free(dlist *l, void (*free_fn)(void*))
{
    if (l==NULL)
        return;
    
    for (dlist *i=l; i;)
    {
        dlist *tmp=i->next;
        if (free_fn)
            (*free_fn)(i->data);
        DFREE(i);
        i=tmp;
    };
};

void dlist_unlink(dlist *l)
{
    dlist *left=l->prev;
    dlist *right=l->next;
    
    left->next=right;
    right->prev=left;
    DFREE(l);
};
