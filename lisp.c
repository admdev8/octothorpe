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

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "dmalloc.h"
#include "lisp.h"

#define FILE_OUT stdout
//#define FILE_OUT stderr

obj* obj_byte (byte i)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    rt->t=OBJ_BYTE;
    rt->u.b=i;
    return rt;
};

obj* obj_wyde (wyde i)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    rt->t=OBJ_WYDE;
    rt->u.w=i;
    return rt;
};

obj* obj_tetrabyte (tetrabyte i)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    rt->t=OBJ_TETRABYTE;
    rt->u.tb=i;
    return rt;
};

obj* obj_octabyte (octabyte i)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    rt->t=OBJ_OCTABYTE;
    rt->u.ob=i;
    return rt;
};

obj* obj_REG (REG i)
{
#ifdef _WIN64
    return obj_octabyte(i);
#else
    return obj_tetrabyte(i);
#endif
};

obj* obj_double (double d)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    rt->t=OBJ_DOUBLE;
    rt->u.d=d;
    return rt;
};

obj* obj_wyde_n_times (wyde i, int t)
{
    if (t==0)
        return NULL;
    return cons (obj_wyde(i), obj_wyde_n_times(i, t-1));
};

obj* obj_tetrabyte_n_times (tetrabyte i, int t)
{
    if (t==0)
        return NULL;
    return cons (obj_tetrabyte(i), obj_tetrabyte_n_times(i, t-1));
};

obj* obj_cstring (const char *s)
{
    obj* rt=DCALLOC (obj, 1, "obj");
    rt->t=OBJ_CSTRING;
    rt->u.s=DSTRDUP (s, "s");
    return rt;
};

obj* cons (obj* head, obj* tail)
{
    obj* rt;
    cons_cell* new_cell;

#if 0
    fprintf(FILE_OUT, "cons(");
    obj_dump(head);
    fprintf(FILE_OUT, " ");
    obj_dump(tail);
    fprintf(FILE_OUT, ")");
#endif

    rt=DCALLOC (obj, 1, "obj");
    new_cell=DCALLOC (cons_cell, 1, "cons_cell");
    
    new_cell->head=head;
    new_cell->tail=tail;

    rt->t=OBJ_CONS;
    rt->u.c=new_cell;
    return rt;
};

bool CONSP(obj* o)
{
    assert(o);
    return o->t==OBJ_CONS;
};

obj* car(obj* o)
{
    assert (CONSP(o));
    return o->u.c->head;
};

obj* cdr(obj* o)
{
    assert (CONSP(o));
    return o->u.c->tail;
};

bool obj_is_opaque(obj* o)
{
    assert(o);
    return o->t==OBJ_OPAQUE;
};

void* obj_unpack_opaque(obj* o)
{
    assert(obj_is_opaque(o));
    return o->u.o->ptr;
};

obj* create_obj_opaque(void* ptr, void (*dumper_fn) (void *), void (*free_fn) (void*))
{
    obj_opaque *op=DCALLOC(obj_opaque, 1, "obj_opaque");
    obj *o=DCALLOC(obj, 1, "obj");

    op->ptr=ptr;
    op->dumper_fn=dumper_fn;
    op->free_fn=free_fn;
    o->t=OBJ_OPAQUE;
    o->u.o=op;

    return o;
};

bool LISTP(obj *o)
{
    assert (o);

    if (o->t!=OBJ_CONS)
        return false;

    if (o->u.c->head->t==OBJ_CONS)
        return false;

    if (o->u.c->tail==NULL)
        return true;

    if (o->u.c->tail->t==OBJ_CONS)
        return LISTP (o->u.c->tail);
    else // tail is not cons and not NULL
        return false;
};

void obj_dump_as_list(obj *o)
{
    assert (LISTP(o));

    // treat it as list!

    printf ("(");
    for (obj* c=o; ; c=c->u.c->tail)
    {
        obj_dump(c->u.c->head);
        if (c->u.c->tail==NULL)
            break;
        else
            printf(" ");
    };
    printf (")");
};

void obj_dump(obj *o)
{
    if(o==NULL)
    {
        fprintf (FILE_OUT, "NULL");
        return;
    };

    if (LISTP(o))
    {
        obj_dump_as_list(o);
        return;
    };

    switch (o->t)
    {
        case OBJ_BYTE:
            fprintf (FILE_OUT, "0x%x", o->u.b);
            break;
        case OBJ_WYDE:
            fprintf (FILE_OUT, "0x%x", o->u.w);
            break;
        case OBJ_TETRABYTE:
            fprintf (FILE_OUT, "0x%x", o->u.tb);
            break;
        case OBJ_OCTABYTE:
            fprintf (FILE_OUT, "0x%I64x", o->u.ob);
            break;
        case OBJ_DOUBLE:
            fprintf (FILE_OUT, "%f", o->u.d);
            break;
        case OBJ_CSTRING:
            fprintf (FILE_OUT, "\"%s\"", o->u.s);
            break;
        case OBJ_CONS:
            fprintf (FILE_OUT, "(");
            obj_dump(car(o));
            fprintf(FILE_OUT, " ");
            obj_dump(cdr(o));
            fprintf (FILE_OUT, ")");
            break;
        case OBJ_OPAQUE:
            if (o->u.o->dumper_fn)
                (*o->u.o->dumper_fn)(o->u.o->ptr);
            else
                fprintf (FILE_OUT, "opaque_0x%p", o->u.o->ptr);
            break;
        default:
            assert(0);
            break;
    };
};

unsigned LENGTH (obj *l)
{
    unsigned rt=0;
    
    assert (LISTP(l));

    for (obj* i=l; i; i=cdr(i))
        rt++;
    
    return rt;
};

obj* LAST(obj *l)
{
    assert (l->t==OBJ_CONS);
    if (l->u.c->tail==NULL)
        return l;
    else
        return LAST(l->u.c->tail);
};

obj* NCONC (obj *l1, obj *l2)
{
    assert (CONSP(l2) && "l2 argument should be list too!"); 

    if (l1==NULL)
        return l2;

    // find last element of l1. it should be NULL.
    obj *last=LAST(l1);
    assert(last->t==OBJ_CONS && "nconc: last element of l1 list should be cons cell");
    last->u.c->tail=l2;
    return l1;
};

void obj_free(obj* o)
{
    if(o==NULL)
        return; // be silent, that behavour is the same as in free(NULL);
    switch (o->t)
    {
        case OBJ_CSTRING:
            DFREE (o->u.s);
            break;
        case OBJ_CONS:
            if (o->u.c->head) obj_free (o->u.c->head);
            if (o->u.c->tail) obj_free (o->u.c->tail);
            DFREE(o->u.c);
            break;
        case OBJ_OPAQUE:
            if (o->u.o->free_fn) (*o->u.o->free_fn)(o->u.o->ptr);
            DFREE(o->u.o);
            break;
        case OBJ_BYTE:
        case OBJ_WYDE:
        case OBJ_TETRABYTE:
        case OBJ_OCTABYTE:
        case OBJ_DOUBLE:
            // do nothing
            break;

        default:
            assert(0); // fail if type is unknown: fail if pointer to incorrect memory chunk is passed
            break;
    };
    DFREE(o);
};

void obj_free_conses_of_list(obj* o)
{
    assert (LISTP(o));
    switch (o->t)
    {
        case OBJ_CONS:
            if (o->u.c->tail) obj_free_conses_of_list (o->u.c->tail);
            DFREE(o->u.c);
            break;
        default:
            assert(0);
            break;
    };
    DFREE(o);
};

obj* create_list(obj* o, ...)
{
    va_list args;
    obj *rt=NULL;
    va_start(args, o);

    for (obj* i=o; i; i=va_arg(args, obj*))
        rt=NCONC(rt, cons(i, NULL));

    va_end(args);
    return rt;
};

tetrabyte obj_get_as_tetrabyte(obj* o)
{
    if (o->t!=OBJ_TETRABYTE)
    {
        printf ("%s() o is not OBJ_TETRABYTE. it is=", __func__);
        obj_dump(o);
        exit(0);
    };
    return o->u.tb;
};

octabyte obj_get_as_octabyte(obj* o)
{
    assert (o->t==OBJ_OCTABYTE);
    return o->u.ob;
};

byte obj_get_as_byte(obj* o)
{
    assert (o->t==OBJ_BYTE);
    return o->u.b;
};

wyde obj_get_as_wyde(obj* o)
{
    assert (o->t==OBJ_WYDE);
    return o->u.w;
};
REG obj_get_as_REG(obj* o)
{
#ifdef _WIN64
    return obj_get_as_octabyte(o);
#else
    return obj_get_as_tetrabyte(o);
#endif
};

char* obj_get_as_cstring(obj* o)
{
    assert (o->t==OBJ_CSTRING);
    return o->u.s;
};

void list_of_bytes_to_array (byte** array, unsigned *array_len, obj* o)
{
   int idx;
   obj *i;
   assert (LISTP(o)); 

   *array_len=LENGTH(o);
   *array=DMALLOC(byte, *array_len, "array");
    for (i=o, idx=0; i; i=cdr(i), idx++)
        (*array)[idx]=obj_get_as_byte(car(i));

    //for (int i=0; i<*array_len; i++)
    //    printf ("idx=%d, %02X\n", i, (*array)[i]);
};

void list_of_wydes_to_array (wyde** array, unsigned *array_len, obj* o)
{
   int idx;
   obj *i;
   assert (LISTP(o)); 

   *array_len=LENGTH(o);
   *array=DMALLOC(wyde, *array_len, "array");
    for (i=o, idx=0; i; i=cdr(i), idx++)
        (*array)[idx]=obj_get_as_wyde(car(i));

    //for (int i=0; i<*array_len; i++)
    //    printf ("idx=%d, %04X\n", i, (*array)[i]);
};

/* vim: set expandtab ts=4 sw=4 : */
