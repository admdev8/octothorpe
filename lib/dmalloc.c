/*
 *             _        _   _                           
 *            | |      | | | |                          
 *   ___   ___| |_ ___ | |_| |__   ___  _ __ _ __   ___ 
 *  / _ \ / __| __/ _ \| __| '_ \ / _ \| '__| '_ \ / _ \
 * | (_) | (__| || (_) | |_| | | | (_) | |  | |_) |  __/
 *  \___/ \___|\__\___/ \__|_| |_|\___/|_|  | .__/ \___|
 *                                          | |         
 *                                          |_|
 *
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2013
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/.
 *
 */

#include <stdio.h>
#include "datatypes.h"
#include "oassert.h"
#include <memory.h>
#include <stdlib.h>

#include "logging.h"
#include "dmalloc.h"
#include "rbtree.h"
#include "stuff.h"
#include "memutils.h"
#include "stuff.h"
#include "config.h"

//#define LOGGING
//#define BREAK_ON_UNKNOWN_BLOCK_BEING_FREED

#ifdef OCTOTHORPE_DEBUG
#define ADD_GUARDS
#define DREE_CHK_ONLY_GUARD_BEING_FREED
#endif


//this is slow! use it only for heavy debugging!
//#define DFREE_CHK_ALL_GUARDS

#ifdef ADD_GUARDS
static tetrabyte guard1=0x44332211;
static tetrabyte guard2=0x88776655;
#endif

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
#ifdef OCTOTHORPE_DEBUG
static rbtree* tbl;

static bool tbl_created=false;

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


#endif // OCTOTHORPE_DEBUG

#ifdef ADD_GUARDS
static void add_guards (void *user_ptr, size_t user_size)
{
    memcpy ((byte*)user_ptr-4, &guard1, 4);
    memcpy ((byte*)user_ptr+user_size, &guard2, 4);
};
#endif

void* dmalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname)
{
    void* rt;

#ifdef LOGGING
    fprintf (stderr, "%s(size=%d, filename=%s:%d func=%s struct=%s)\n", __func__, size, filename, line, function, structname);
#endif

    if (break_on_seq_n && (seq_n==seq_n_to_break_on))
        debugger_breakpoint();

#ifdef ADD_GUARDS
    rt=malloc (size+8)+4;
#else    
    rt=malloc (size);
#endif    
    
    if (rt==NULL)
        die("%s() can't allocate size %d for %s (%s:%d)\n", __func__, size, structname, filename, line);

#ifdef OCTOTHORPE_DEBUG
    tetrabytefill (rt, size, 0x0BADF00D); // poison #1
#endif

#ifdef ADD_GUARDS
    add_guards (rt, size);
#endif     

#ifdef OCTOTHORPE_DEBUG
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
#ifdef OCTOTHORPE_DEBUG
    struct dmalloc_info *tmp;
#endif

#ifdef LOGGING
    fprintf (stderr, "%s(ptr=0x%p, size=%d, filename=%s:%d func=%s struct=%s)\n", __func__, ptr, size, filename, line, function, structname);
#endif

    if (ptr==NULL)
        return dmalloc (size, filename, line, function, structname);
    if (size==0)
    {
        DFREE(ptr);
        return NULL;
    };

#ifdef ADD_GUARDS
    newptr=(byte*)realloc ((byte*)ptr-4, size+8);
#else    
    newptr=realloc (ptr, size);
#endif 

    if (newptr==NULL)
        die("%s() can't allocate size %d for %s (%s:%d)\n", __FUNCTION__, size, structname, filename, line);

#ifdef ADD_GUARDS    
    newptr=newptr+4;
    add_guards(newptr, size);
#endif 

#ifdef OCTOTHORPE_DEBUG
    if (newptr!=ptr)
    {
        store_info (newptr, size, filename, line, function, structname);

        tmp=rbtree_lookup(tbl, ptr);
        oassert(tmp && "drealloc(ptr): ptr isn't present in our records"); // ensure it's present
        free(tmp);
        rbtree_delete(tbl, ptr);
    }
    else
    {
        tmp=rbtree_lookup(tbl, ptr);
        oassert(tmp && "drealloc(ptr): ptr isn't present in our records"); // ensure it's present
        tmp->user_size=size; // set new size
    };
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

char* dstrdup (const char *str, const char * filename, unsigned line, const char * function, const char * structname)
{
    size_t len;
    void *newp;

    oassert (str);

    len=strlen(str)+1;

    newp=dmalloc (len, filename, line, function, structname);
    memcpy (newp, str, len);

    return newp;
};

#ifdef ADD_GUARDS
static void chk_guard (void *ptr, struct dmalloc_info *i)
{
    int r1, r2;
    size_t size=i->user_size;

    r1=memcmp ((byte*)ptr-4, &guard1, 4);
    r2=memcmp ((byte*)ptr+size, &guard2, 4);

    if (r1!=0 || r2!=0)
    {
        printf ("%s(): %s %s overwritten for block:", 
                __func__,
                r1!=0 ? "guard1" : "",
                r2!=0 ? "guard2" : "");
        dump_blk_info (i);
        if (r1!=0) 
            printf ("guard1=%08X, should be=%08X\n", *(tetrabyte*)((byte*)ptr-4), guard1);
        if (r2!=0) 
            printf ("guard2=%08X, should be=%08X\n", *(tetrabyte*)((byte*)ptr+size), guard2);
        L_init("tmp");
        L ("block with both guards:\n");
        L_print_buf((byte*)ptr-4, size+8);
        die ("exiting\n");
    };
};
#endif

#ifdef DFREE_CHK_ALL_GUARDS
static void chk_all_guards()
{
#ifdef LOGGING
    fprintf (stderr, __func__"()\n");
#endif
    rbtree_foreach(tbl, chk_guard, NULL, NULL);
};
#endif

void dfree (void* ptr)
{
    dfree2 (ptr, __FILE__, __LINE__, __func__);
};

void dfree2 (void* ptr, const char *filename, unsigned line, const char *funcname)
{
#ifdef OCTOTHORPE_DEBUG
    struct dmalloc_info *tmp;
    size_t blk_user_size=0;
#endif

#ifdef LOGGING 
    fprintf (stderr, "%s(ptr=0x%p filename=%s line=%d funcname=%s)\n", __func__, ptr, filename, line, funcname);
#endif    
    
    if (ptr==NULL)
        return; // do nothing - by standard

#ifdef DFREE_CHK_ALL_GUARDS
    chk_all_guards();
#endif

#ifdef DREE_CHK_ONLY_GUARD_BEING_FREED
    tmp=rbtree_lookup(tbl, ptr);
    if (tmp)
        chk_guard (ptr, tmp);
#endif

#ifdef OCTOTHORPE_DEBUG
    //printf ("dfree (0x%p)\n", ptr);
    tmp=rbtree_lookup(tbl, ptr);

    //oassert(tmp && "dfree(ptr): ptr isn't present in our records"); // ensure it's present
    if (tmp==NULL)
    {
        fprintf (stderr, "%s(0x%p): ptr isn't present in our records\n", __FUNCTION__, ptr);
        fprintf (stderr, "filename:line=%s:%d, function=%s\n", filename, line, funcname);
#ifdef BREAK_ON_UNKNOWN_BLOCK_BEING_FREED
        debugger_breakpoint();
#endif
    }
    else
    {
        blk_user_size=tmp->user_size;
        free(tmp);
    };
    rbtree_delete(tbl, ptr);
#endif

#ifdef ADD_GUARDS
#ifdef OCTOTHORPE_DEBUG
    tetrabytefill (ptr, blk_user_size, 0xB1CF1EED); // poison #2
#endif
    free ((byte*)ptr-4);
#else
    free (ptr);
#endif    
};


#ifdef OCTOTHORPE_DEBUG
static void dump_unfreed_block(void *k, struct dmalloc_info *i)
{    
    fds _fds={ NULL, NULL};
    _fds.fd1=stdout;

    oassert(k!=NULL);
    oassert(i!=NULL);
    dump_blk_info (i);
    
    L_print_buf_ofs_fds (&_fds, k, i->user_size < 0x20 ? i->user_size : 0x20, k);
};
#endif

void* dmemdup (void *p, size_t s, const char * filename, unsigned line, const char * function, const char * structname)
{
    oassert (p);
    void *rt=dmalloc (s, filename, line, function, structname);
    oassert(rt!=NULL);
    memcpy(rt, p, s);
    return rt;
};

void dump_unfreed_blocks()
{
#ifdef OCTOTHORPE_DEBUG
    rbtree_foreach(tbl, (void(*)(void*,void*))dump_unfreed_block, NULL, NULL);
#endif    
};

void dmalloc_deinit()
{
#ifdef OCTOTHORPE_DEBUG
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

void* memdup_range (void *s, size_t size)
{
    void *rt;

    rt=DMALLOC(char, size, "char");
    memcpy (rt, s, size);
    
    return rt;
};

char *strndup (const char *s, size_t size)
{
    char *rt;

    rt=DMALLOC(char, size+1, "char"); // including terminate zero
    memcpy (rt, s, size);
    rt[size]=0;

    return rt;
};

/* vim: set expandtab ts=4 sw=4 : */
