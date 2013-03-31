//#include <windows.h> do you need it here?
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "dmalloc.h"
#include "bool.h"
#include "rbtree.h"

#include <intrin.h>

// TODO: а еще тут может быть slab allocator! http://en.wikipedia.org/wiki/Slab_allocation

// TODO: еще нужна ф-ция для вываливания статистики, сколько блоков такого-то типа и сколько места они занимают

//BOOL break_on_seq_n=TRUE;
BOOL break_on_seq_n=FALSE;
unsigned seq_n_to_break_on=1519;

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
static rbtree tbl;
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

    if (break_on_seq_n && (seq_n==seq_n_to_break_on))
        __debugbreak();

    rt=malloc (size);
    
    if (rt==NULL)
        die(__FUNCTION__"() can't allocate size %d for %s (%s:%d)\n", size, structname, filename, line);

#ifdef _DEBUG    
    store_info (rt, size, filename, line, function, structname);
#endif    
    return rt;
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
    //printf ("dfree (0x%p)\n", ptr);
    tmp=rbtree_lookup(tbl, ptr);
    assert(tmp && "dfree(ptr): ptr isn't present in our records"); // ensure it's present
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
    rbtree_foreach(tbl, dump_unfreed_block);
#endif    
};

void dmalloc_deinit()
{
#ifdef _DEBUG    
    rbtree_clear(tbl);
    tbl_created=FALSE;
#endif    
};

#ifdef TEST

void main()
{
    void* t1=DMALLOC(123, "block123");
    void* t2=DMALLOC(124, "block124");
    void* t3=DMALLOC(12,  "block12");
    void* t4=DMALLOC(555, "block555");
    void* t5=DMALLOC(666, "block666");
    void* t6=DMALLOC(777, "block777");

    dfree(t1);
    dfree(t5);
    dfree(t6);

    dump_unfreed_blocks();
    dmalloc_deinit();
};

#endif
