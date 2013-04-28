#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#include "logging.h"
#include "dmalloc.h"
#include "rbtree.h"
#include "stuff.h"
#include "memutils.h"

//#define LOGGING

#define ADD_GUARDS

#define DREE_CHK_ONLY_GUARD_BEING_FREED

//this is slow! use it only for heavy debugging!
//#define DFREE_CHK_ALL_GUARDS

#ifdef ADD_GUARDS
static uint32_t guard1=0x44332211;
static uint32_t guard2=0x88776655;
#endif

// !!! Idea: он может строить статистку наболее часто выделяемых блоков!

// TODO: а еще тут может быть slab allocator! http://en.wikipedia.org/wiki/Slab_allocation

// TODO: еще нужна ф-ция для вываливания статистики, сколько блоков такого-то типа и сколько места они занимают

static bool break_on_seq_n=false;
static unsigned seq_n_to_break_on;

static unsigned seq_n=0;

struct dmalloc_info
{
    unsigned seq_n;
    size_t user_size;
    const char *filename;
    unsigned line;
    const char *function;
    const char *structname;
};

// in ADD_GUARDS case, ptr and size stored here is from user's perspective...
#ifdef _DEBUG
static rbtree* tbl;
#endif

static bool tbl_created=false;

#ifdef _DEBUG
void store_info (void* user_ptr, size_t user_size, const char * filename, unsigned line, const char * function, 
        const char * structname)
{
    struct dmalloc_info *tmp;
    tmp=(struct dmalloc_info*)calloc(1, sizeof(struct dmalloc_info));
    tmp->seq_n=seq_n;
    seq_n++;
    tmp->user_size=user_size;
    tmp->filename=filename;
    tmp->line=line;
    tmp->function=function;
    tmp->structname=structname;
    //printf ("adding ptr=0x%p\n", ptr);
    if (tbl_created==false)
    {
        tbl=rbtree_create(false, NULL, compare_size_t);
        tbl_created=true;
    };
    rbtree_insert (tbl, user_ptr, tmp);
};

static void dump_blk_info (struct dmalloc_info *i)
{
    printf ("seq_n:%d, size: %d, filename: %s:%d, func: %s, struct: %s\n", i->seq_n, i->user_size, i->filename, i->line, i->function, i->structname);
};

#endif

static void add_guards (void *user_ptr, size_t user_size)
{
    memcpy ((uint8_t*)user_ptr-4, &guard1, 4);
    memcpy ((uint8_t*)user_ptr+user_size, &guard2, 4);
};

void* dmalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname)
{
    void* rt;

#ifdef LOGGING
    fprintf (stderr, __func__"(size=%d, filename=%s:%d func=%s struct=%s)\n", size, filename, line, function, structname);
#endif

    if (break_on_seq_n && (seq_n==seq_n_to_break_on))
        debugger_breakpoint();

#ifdef ADD_GUARDS
    rt=(uint8_t*)malloc (size+8)+4;
#else    
    rt=malloc (size);
#endif    
    
    if (rt==NULL)
        die("%s() can't allocate size %d for %s (%s:%d)\n", __func__, size, structname, filename, line);

#ifdef ADD_GUARDS
    add_guards (rt, size);
#endif    

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
    fprintf (stderr, __func__"(ptr=0x%p, size=%d, filename=%s:%d func=%s struct=%s)\n", ptr, size, filename, line, function, structname);
#endif

    if (ptr==NULL)
        return dmalloc (size, filename, line, function, structname);
    if (size==0)
    {
        dfree(ptr);
        return NULL;
    };

#ifdef _DEBUG    
#ifdef ADD_GUARDS    
    newptr=(uint8_t*)realloc ((uint8_t*)ptr-4, size+8)+4;
#else    
    newptr=realloc (ptr, size);
#endif 

    if (newptr==NULL)
        die("%s() can't allocate size %d for %s (%s:%d)\n", __FUNCTION__, size, structname, filename, line);

#ifdef ADD_GUARDS    
    add_guards(newptr, size);
#endif 

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
        tmp->user_size=size; // set new size
    };
#else
    newptr=realloc (ptr, size);
#endif

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

#ifdef ADD_GUARDS
static void chk_guard (void *ptr, struct dmalloc_info *i)
{
    int r1, r2;
    size_t size=i->user_size;

    r1=memcmp ((uint8_t*)ptr-4, &guard1, 4);
    r2=memcmp ((uint8_t*)ptr+size, &guard2, 4);

    if (r1!=0 || r2!=0)
    {
        printf ("%s(): %s %s overwritten for block:", 
                __FUNCTION__,
                r1!=0 ? "guard1" : "",
                r2!=0 ? "guard2" : "");
        dump_blk_info (i);
        if (r1!=0) 
            printf ("guard1=%08X, should be=%08X\n", *(uint32_t*)((uint8_t*)ptr-4), guard1);
        if (r2!=0) 
            printf ("guard2=%08X, should be=%08X\n", *(uint32_t*)((uint8_t*)ptr+size), guard2);
        L_init("tmp");
        L ("block with both guards:\n");
        L_print_buf((uint8_t*)ptr-4, size+8);
        die ("exiting\n");
    };
};
#endif

static void chk_all_guards()
{
#ifdef LOGGING
    fprintf (stderr, __func__"()\n");
#endif
    rbtree_foreach(tbl, chk_guard, NULL, NULL);
};

void dfree (void* ptr)
{
#ifdef _DEBUG
    struct dmalloc_info *tmp;
#endif

#ifdef LOGGING 
    fprintf (stderr, __func__"(ptr=0x%p)\n", ptr);
#endif    
    
    if (ptr==NULL)
        return;

#ifdef DFREE_CHK_ALL_GUARDS
    chk_all_guards();
#endif

#ifdef DREE_CHK_ONLY_GUARD_BEING_FREED
    tmp=rbtree_lookup(tbl, ptr);
    if (tmp==NULL)
        fprintf (stderr, "%s(0x%p): ptr isn't present in our records\n", __FUNCTION__, ptr);
    else
        chk_guard (ptr, tmp);
#endif

#ifdef _DEBUG    
    //printf ("dfree (0x%p)\n", ptr);
    tmp=rbtree_lookup(tbl, ptr);

    //assert(tmp && "dfree(ptr): ptr isn't present in our records"); // ensure it's present
    if (tmp==NULL)
        fprintf (stderr, "%s(0x%p): ptr isn't present in our records\n", __FUNCTION__, ptr);
    else
        free(tmp);
    rbtree_delete(tbl, ptr);
#endif

#ifdef ADD_GUARDS
    free ((uint8_t*)ptr-4);
#else
    free (ptr);
#endif    
};


#ifdef _DEBUG
static void dump_unfreed_block(void *k, struct dmalloc_info *i)
{    
    assert(k!=NULL);
    assert(i!=NULL);
    dump_blk_info (i);
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
        rbtree_foreach (tbl, NULL, NULL, free);
        rbtree_clear(tbl);
        tbl_created=false;
    };
#endif    
};

void dmalloc_break_at_seq_n (unsigned seq_n)
{
    break_on_seq_n=true;
    seq_n_to_break_on=seq_n;
};
