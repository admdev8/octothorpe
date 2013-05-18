#pragma once

typedef struct _dlist
{
    struct _dlist* prev;
    struct _dlist* next;
    void* data;
} dlist;

dlist *dlist_init();
void dlist_insert_at_begin(dlist *l, void *data);
void* dlist_get_first(dlist *l);
void dlist_free(dlist *l, void (*free_fn)(void*));
void dlist_unlink(dlist *l);

