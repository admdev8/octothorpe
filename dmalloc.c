#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "dmalloc.h"
#include "bool.h"
#include "rbtree.h"

#include <intrin.h>

// а еще тут может быть slab allocator! http://en.wikipedia.org/wiki/Slab_allocation

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

static rbtree tbl;
static BOOL tbl_created=FALSE;

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
        tbl=rbtree_create();
        tbl_created=TRUE;
    };
    rbtree_insert (tbl, ptr, tmp, compare_size_t);
};

void* dmalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname)
{
    void* rt;

    if (break_on_seq_n && (seq_n==seq_n_to_break_on))
        __debugbreak();

    rt=malloc (size);
    store_info (rt, size, filename, line, function, structname);
    return rt;
};

void* dcalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname)
{
    void* rt=dmalloc (size, filename, line, function, structname);
    bzero (rt, size);
    return rt;
};

void dfree (void* ptr)
{
#ifdef _DEBUG
    struct dmalloc_info *tmp;
    //printf ("dfree (0x%p)\n", ptr);
    tmp=rbtree_lookup(tbl, ptr, compare_size_t);
    assert(tmp); // ensure it's present
    free(tmp);
    rbtree_delete(tbl, ptr, compare_size_t);
#endif
    free (ptr);
};

static void dump_unfreed_block(void *k, struct dmalloc_info *i)
{
    assert(k!=NULL);
    assert(i!=NULL);
    printf ("seq_n:%d, size: %d, filename: %s:%d, func: %s, struct: %s\n", 
            i->seq_n, i->size, i->filename, i->line, i->function, i->structname);
};

// TODO: void dump_statistics(); // как в Oracle

void dump_unfreed_blocks()
{
    rbtree_enumerate(tbl, dump_unfreed_block);
};

void dmalloc_deinit()
{
    rbtree_clear(tbl);
    tbl_created=FALSE;
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
