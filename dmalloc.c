#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "dmalloc.h"
#include "bool.h"
#include "rbtree.h"

#include <intrin.h>

//#define LOGGING

// !!! Idea: он может строить статистку наболее часто выделяемых блоков!

// TODO: а еще тут может быть slab allocator! http://en.wikipedia.org/wiki/Slab_allocation

// TODO: еще нужна ф-ция для вываливания статистики, сколько блоков такого-то типа и сколько места они занимают

static BOOL break_on_seq_n=FALSE;
static unsigned seq_n_to_break_on;

static unsigned seq_n=0;

struct dmalloc_info
{
    unsigned seq_n;
    size_t size;
    const char *filename;
    unsigned line;
    const char *function;
    const char *structname;
};

#ifdef _DEBUG
static rbtree* tbl;
#endif

static BOOL tbl_created=FALSE;

#ifdef _DEBUG
void store_info (void* ptr, size_t size, const char * filename, unsigned line, const char * function, 
        const char * structname)
{
    struct dmalloc_info *tmp;
    tmp=(struct dmalloc_info*)calloc(1, sizeof(struct dmalloc_info));
    tmp->seq_n=seq_n;
    seq_n++;
    tmp->size=size;
    tmp->filename=filename;
    tmp->line=line;
    tmp->function=function;
    tmp->structname=structname;
    //printf ("adding ptr=0x%p\n", ptr);
    if (tbl_created==FALSE)
    {
        tbl=rbtree_create(FALSE, NULL, compare_size_t);
        tbl_created=TRUE;
    };
    rbtree_insert (tbl, ptr, tmp);
};
#endif

void* dmalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname)
{
    void* rt;

#ifdef LOGGING
    fprintf (stderr, __FUNCTION__"(size=%d, filename=%s:%d func=%s struct=%s)\n", size, filename, line, function, structname);
#endif

    if (break_on_seq_n && (seq_n==seq_n_to_break_on))
        __debugbreak();

    rt=malloc (size);
    
    if (rt==NULL)
        die(__FUNCTION__"() can't allocate size %d for %s (%s:%d)\n", size, structname, filename, line);

#ifdef _DEBUG    
    store_info (rt, size, filename, line, function, structname);
#endif

#ifdef LOGGING
    fprintf (stderr, "returning 0x%p\n", rt);
#endif
    return rt;
};

void* drealloc (void* ptr, size_t size, const char * filename, unsigned line, const char * function, const char * structname)
{
    void* newptr;
#ifdef _DEBUG
    struct dmalloc_info *tmp;
#endif

#ifdef LOGGING
    fprintf (stderr, __FUNCTION__"(ptr=0x%p, size=%d, filename=%s:%d func=%s struct=%s)\n", ptr, size, filename, line, function, structname);
#endif

    if (ptr==NULL)
        return dmalloc (size, filename, line, function, structname);
    if (size==0)
    {
        dfree(ptr);
        return NULL;
    };

#ifdef _DEBUG    
    newptr=realloc (ptr, size);

    if (newptr==NULL)
        die(__FUNCTION__"() can't allocate size %d for %s (%s:%d)\n", size, structname, filename, line);

    if (newptr!=ptr)
    {
        store_info (newptr, size, filename, line, function, structname);

        tmp=rbtree_lookup(tbl, ptr);
        assert(tmp && "drealloc(ptr): ptr isn't present in our records"); // ensure it's present
        free(tmp);
        rbtree_delete(tbl, ptr);
    }
    else
    {
        tmp=rbtree_lookup(tbl, ptr);
        assert(tmp && "drealloc(ptr): ptr isn't present in our records"); // ensure it's present
        tmp->size=size; // set new size
    };
#else
    newptr=realloc (ptr, size);
#endif

exit_and_return:
#ifdef LOGGING
    fprintf (stderr, "returning 0x%p\n", newptr);
#endif
    return newptr;
};

void* dcalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname)
{
    void* rt=dmalloc (size, filename, line, function, structname);
    bzero (rt, size);
    return rt;
};

char* dstrdup (const char *s, const char * filename, unsigned line, const char * function, const char * structname)
{
    size_t len;
    void *newp;

    len=strlen(s)+1;

    newp=dmalloc (len, filename, line, function, structname);
    memcpy (newp, s, len);

    return newp;
};

void dfree (void* ptr)
{
#ifdef _DEBUG
    struct dmalloc_info *tmp;
#endif

#ifdef LOGGING 
    fprintf (stderr, __FUNCTION__"(ptr=0x%p)\n", ptr);
#endif    

    if (ptr==NULL)
        return;

#ifdef _DEBUG    
    //printf ("dfree (0x%p)\n", ptr);
    tmp=rbtree_lookup(tbl, ptr);
    //assert(tmp && "dfree(ptr): ptr isn't present in our records"); // ensure it's present
    if (tmp==NULL)
        fprintf (stderr, __FUNCTION__"(0x%p): ptr isn't present in our records\n", ptr);
    else
        free(tmp);
    rbtree_delete(tbl, ptr);
#endif
    free (ptr);
};

#ifdef _DEBUG
static void dump_unfreed_block(void *k, struct dmalloc_info *i)
{    
    assert(k!=NULL);
    assert(i!=NULL);
    printf ("seq_n:%d, size: %d, filename: %s:%d, func: %s, struct: %s\n", 
            i->seq_n, i->size, i->filename, i->line, i->function, i->structname);
};
#endif

void* dmemdup (void *p, size_t s, const char * filename, unsigned line, const char * function, const char * structname)
{
    void *rt=dmalloc (s, filename, line, function, structname);
    assert(rt!=NULL);
    memcpy(rt, p, s);
    return rt;
};

// TODO: void dump_statistics(); // как в Oracle

void dump_unfreed_blocks()
{
#ifdef _DEBUG    
    rbtree_foreach(tbl, dump_unfreed_block, NULL, NULL);
#endif    
};

void dmalloc_deinit()
{
#ifdef _DEBUG    
    if (tbl_created)
    {
        rbtree_clear(tbl);
        tbl_created=FALSE;
    };
#endif    
};

void dmalloc_break_at_seq_n (unsigned seq_n)
{
    break_on_seq_n=TRUE;
    seq_n_to_break_on=seq_n;
};
