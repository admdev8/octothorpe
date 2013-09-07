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
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "oassert.h"
#include "dmalloc.h"
#include "lisp.h"

#define FILE_OUT stdout
//#define FILE_OUT stderr

void obj_byte2 (byte i, obj* o)
{
    o->t=OBJ_BYTE;
    o->u.b=i;
};

void obj_wyde2 (wyde i, obj* o)
{
    o->t=OBJ_WYDE;
    o->u.w=i;
};

void obj_tetrabyte2 (tetrabyte i, obj* o)
{
    o->t=OBJ_TETRABYTE;
    o->u.tb=i;
};

void obj_octabyte2 (octabyte i, obj* o)
{
    o->t=OBJ_OCTABYTE;
    o->u.ob=i;
};

void obj_REG2 (REG i, obj* o)
{
#ifdef _WIN64
    obj_octabyte2 (i, o);
#else
    obj_tetrabyte2 (i, o);
#endif
};

void obj_double2 (double d, obj* o)
{
    o->t=OBJ_DOUBLE;
    o->u.d=d;
};

void obj_xmm2 (byte *ptr, obj *o)
{
    o->t=OBJ_XMM;
    o->u.xmm=DMEMDUP(ptr, 16, "XMM value");
};

obj* obj_byte (byte i)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    obj_byte2 (i, rt);
    return rt;
};

obj* obj_wyde (wyde i)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    obj_wyde2(i, rt);
    return rt;
};

obj* obj_tetrabyte (tetrabyte i)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    obj_tetrabyte2(i, rt);
    return rt;
};

obj* obj_octabyte (octabyte i)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    obj_octabyte2 (i, rt);
    return rt;
};

obj* obj_REG (REG i)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    obj_REG2 (i, rt);
    return rt;
};

obj* obj_double (double d)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    obj_double2 (d, rt);
    return rt;
};

obj* obj_xmm (byte *ptr)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    obj_xmm2 (ptr, rt);
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
    oassert(o);
    return o->t==OBJ_CONS;
};

obj* car(obj* o)
{
    oassert (CONSP(o));
    return o->u.c->head;
};

obj* cdr(obj* o)
{
    oassert (CONSP(o));
    return o->u.c->tail;
};

bool obj_is_opaque(obj* o)
{
    oassert(o);
    return o->t==OBJ_OPAQUE;
};

void* obj_unpack_opaque(obj* o)
{
    oassert(obj_is_opaque(o));
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
    oassert (o);

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
    oassert (LISTP(o));

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
        case OBJ_XMM:
            printf ("0x");
            for (int i=0; i<16; i++)
                fprintf (FILE_OUT, "%02X", o->u.xmm[i]);
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
            oassert(0);
            break;
    };
};

// shallow copy
void obj_copy2 (obj *dst, obj *src)
{
    switch (src->t)
    {
        case OBJ_NONE:
            break;
        case OBJ_BYTE:
            dst->u.b=src->u.b;
            break;
        case OBJ_WYDE:
            dst->u.w=src->u.w;
            break;
        case OBJ_TETRABYTE:
            dst->u.tb=src->u.tb;
            break;
        case OBJ_OCTABYTE:
            dst->u.ob=src->u.ob;
            break;
        case OBJ_DOUBLE:
            dst->u.d=src->u.d;
            break;
        case OBJ_XMM:
            dst->u.xmm=DMEMDUP (src->u.xmm, 16, "xmm");
            break;
        case OBJ_CSTRING:
            dst->u.s=DSTRDUP (src->u.s, "s");
        case OBJ_CONS:
            oassert (!"cons object copying isn't yet supported");
            break;
        case OBJ_OPAQUE:
            oassert (!"opaque object copying isn't yet supported");
            break;
        default:
            oassert (!"something unsupported");
            break;
    };
    dst->t=src->t;
};

bool EQL(obj *o1, obj* o2)
{
    if(o1==NULL && o2==NULL)
        return true;

    if (o1->t!=o2->t)
        return false;

    switch (o1->t)
    {
        case OBJ_BYTE:
            return obj_get_as_byte(o1)==obj_get_as_byte(o2);

        case OBJ_WYDE:
            return obj_get_as_wyde(o1)==obj_get_as_wyde(o2);

        case OBJ_TETRABYTE:
            return obj_get_as_tetrabyte(o1)==obj_get_as_tetrabyte(o2);
            
        case OBJ_OCTABYTE:
            return obj_get_as_octabyte(o1)==obj_get_as_octabyte(o2);

        case OBJ_DOUBLE:
            return o1->u.d==o2->u.d;

        case OBJ_XMM:
            return memcmp (o1->u.xmm, o2->u.xmm, 16)==0;

        case OBJ_CSTRING:
            return strcmp (o1->u.s, o2->u.s)==0;

        case OBJ_CONS:
            return car(o1)==car(o2) && cdr(o1)==cdr(o2); // this is EQL, remember

        case OBJ_OPAQUE:
            oassert (!"opaque objects are not supported in EQL");
            fatal_error();

        default:
            oassert(!"unknown type");
            fatal_error();
    };
};

unsigned LENGTH (obj *l)
{
    unsigned rt=0;
    
    oassert (LISTP(l));

    for (obj* i=l; i; i=cdr(i))
        rt++;
    
    return rt;
};

obj* LAST(obj *l)
{
    oassert (l->t==OBJ_CONS);
    if (l->u.c->tail==NULL)
        return l;
    else
        return LAST(l->u.c->tail);
};

obj* NCONC (obj *l1, obj *l2)
{
    oassert (CONSP(l2) && "l2 argument should be list too!"); 

    if (l1==NULL)
        return l2;

    // find last element of l1. it should be NULL.
    obj *last=LAST(l1);
    oassert(last->t==OBJ_CONS && "nconc: last element of l1 list should be cons cell");
    last->u.c->tail=l2;
    return l1;
};

void obj_free_structures(obj* o)
{
    if(o==NULL)
        return; // be silent, that behavour is the same as in free(NULL);
    switch (o->t)
    {
        case OBJ_CSTRING:
            DFREE (o->u.s);
            break;
        case OBJ_XMM:
            DFREE (o->u.xmm);
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
        case OBJ_NONE:
        case OBJ_BYTE:
        case OBJ_WYDE:
        case OBJ_TETRABYTE:
        case OBJ_OCTABYTE:
        case OBJ_DOUBLE:
            // do nothing
            break;

        default:
            oassert(!"type unknown"); // fail if type is unknown: fail if pointer to incorrect memory chunk is passed
            break;
    };
};

void obj_free(obj* o)
{
    obj_free_structures(o);
    DFREE(o);
};

void obj_free_conses_of_list(obj* o)
{
    oassert (LISTP(o));
    switch (o->t)
    {
        case OBJ_CONS:
            if (o->u.c->tail) obj_free_conses_of_list (o->u.c->tail);
            DFREE(o->u.c);
            break;
        default:
            oassert(0);
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
    oassert (o->t==OBJ_TETRABYTE);
    return o->u.tb;
};

double obj_get_as_double(obj* o)
{
    oassert (o->t==OBJ_DOUBLE);
    return o->u.d;
};

octabyte obj_get_as_octabyte(obj* o)
{
    oassert (o->t==OBJ_OCTABYTE);
    return o->u.ob;
};

byte obj_get_as_byte(obj* o)
{
    oassert (o->t==OBJ_BYTE);
    return o->u.b;
};

wyde obj_get_as_wyde(obj* o)
{
    oassert (o->t==OBJ_WYDE);
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

octabyte zero_extend_to_octabyte(obj* o)
{
    switch (o->t)
    {
        case OBJ_BYTE:
            return o->u.b;
        case OBJ_WYDE:
            return o->u.w;
        case OBJ_TETRABYTE:
            return o->u.tb;
        case OBJ_OCTABYTE:
            return o->u.ob;
        default:
            oassert(!"other types are not convertible to octabyte");
            fatal_error();
    };
};

REG zero_extend_to_REG(obj* o)
{
    switch (o->t)
    {
        case OBJ_BYTE:
            return o->u.b;
        case OBJ_WYDE:
            return o->u.w;
        case OBJ_TETRABYTE:
            return o->u.tb;
        case OBJ_OCTABYTE:
#ifdef _WIN64
            return o->u.ob;
#else
            oassert(!"cannot convert octabyte to REG");
#endif            
            fatal_error();
        default:
            oassert(!"other types are not convertible to REG");
            fatal_error();
    };
};

bool obj_is_zero(obj* o)
{
    switch (o->t)
    {
        case OBJ_BYTE:
            return o->u.b==0;
        case OBJ_WYDE:
            return o->u.w==0;
        case OBJ_TETRABYTE:
            return o->u.tb==0;
        case OBJ_OCTABYTE:
            return o->u.ob==0;
        default:
            oassert(!"other types are not supported (so far)");
            fatal_error();
    };
};

char* obj_get_as_cstring(obj* o)
{
    oassert (o->t==OBJ_CSTRING);
    return o->u.s;
};

byte* obj_get_as_xmm(obj* o)
{
    oassert (o->t==OBJ_XMM);
    return o->u.xmm;
};

void list_of_bytes_to_array (byte** array, unsigned *array_len, obj* o)
{
   int idx;
   obj *i;
   oassert (LISTP(o)); 

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
   oassert (LISTP(o)); 

   *array_len=LENGTH(o);
   *array=DMALLOC(wyde, *array_len, "array");
    for (i=o, idx=0; i; i=cdr(i), idx++)
        (*array)[idx]=obj_get_as_wyde(car(i));

    //for (int i=0; i<*array_len; i++)
    //    printf ("idx=%d, %04X\n", i, (*array)[i]);
};

void obj_REG2_and_set_type(enum obj_type t, REG v, obj* out)
{
    switch (t)
    {
        case OBJ_BYTE:      
            obj_byte2(v&0xFF, out); break;
        case OBJ_WYDE:      
            obj_wyde2(v&0xFFFF, out); break;
        case OBJ_TETRABYTE: 
            obj_tetrabyte2(v&0xFFFFFFFF, out); break;
        case OBJ_OCTABYTE:  
            obj_octabyte2(v, out); break;
        default:
            oassert(!"other types are not supported");
            fatal_error();
    };
};

int get_2nd_most_significant_bit(obj *i)
{
    switch (i->t)
    {
        case OBJ_OCTABYTE:
            return (obj_get_as_octabyte(i) & 0x4000000000000000) ? 1 : 0;
        case OBJ_TETRABYTE:
            return (obj_get_as_tetrabyte(i) & 0x40000000) ? 1 : 0;
        case OBJ_WYDE:
            return (obj_get_as_wyde(i) & 0x4000) ? 1 : 0;
        case OBJ_BYTE:
            return (obj_get_as_byte(i) & 0x40) ? 1 : 0;
        default:
            oassert(!"unsupported type");
            fatal_error();
    };
};

int get_lowest_byte(obj *i)
{
    switch (i->t)
    {
        case OBJ_OCTABYTE:
            return obj_get_as_octabyte(i) & 0xFF;
        case OBJ_TETRABYTE:
            return obj_get_as_tetrabyte(i) & 0xFF;
        case OBJ_WYDE:
            return obj_get_as_wyde(i) & 0xFF;
        case OBJ_BYTE:
            return obj_get_as_byte(i);
        default:
            oassert(!"unsupported type");
            fatal_error();
    };
};

bool obj_get_4th_bit(obj *i)
{
    return (get_lowest_byte(i)>>4) & 1;
};

void obj_decrement(obj *i)
{
    switch (i->t)
    {
        case OBJ_OCTABYTE:
            i->u.ob--;
            break;
        case OBJ_TETRABYTE:
            i->u.tb--;
            break;
        case OBJ_WYDE:
            i->u.w--;
            break;
        case OBJ_BYTE:
            i->u.b--;
            break;
        default:
            oassert(!"unsupported type");
    };
};

void obj_subtract(obj *op1, obj *op2, obj *result)
{
    oassert (op1->t==op2->t);

    switch (op1->t)
    {
        case OBJ_OCTABYTE:
            obj_octabyte2 (op1->u.ob - op2->u.ob, result);
            break;
        case OBJ_TETRABYTE:
            obj_tetrabyte2 (op1->u.tb - op2->u.tb, result);
            break;
        case OBJ_WYDE:
            obj_wyde2 (op1->u.w - op2->u.w, result);
            break;
        case OBJ_BYTE:
            obj_byte2 (op1->u.b - op2->u.b, result);
            break;
        default:
            oassert(!"unsupported type");
    };
};

void obj_add(obj *op1, obj *op2, obj *result)
{
    oassert (op1->t==op2->t);

    switch (op1->t)
    {
        case OBJ_OCTABYTE:
            obj_octabyte2 (op1->u.ob + op2->u.ob, result);
            break;
        case OBJ_TETRABYTE:
            obj_tetrabyte2 (op1->u.tb + op2->u.tb, result);
            break;
        case OBJ_WYDE:
            obj_wyde2 (op1->u.w + op2->u.w, result);
            break;
        case OBJ_BYTE:
            obj_byte2 (op1->u.b + op2->u.b, result);
            break;
        default:
            oassert(!"unsupported type");
    };
};

void obj_XOR(obj *op1, obj *op2, obj *result)
{
    oassert (op1->t==op2->t);

    switch (op1->t)
    {
        case OBJ_OCTABYTE:
            obj_octabyte2 (op1->u.ob ^ op2->u.ob, result);
            break;
        case OBJ_TETRABYTE:
            obj_tetrabyte2 (op1->u.tb ^ op2->u.tb, result);
            break;
        case OBJ_WYDE:
            obj_wyde2 (op1->u.w ^ op2->u.w, result);
            break;
        case OBJ_BYTE:
            obj_byte2 (op1->u.b ^ op2->u.b, result);
            break;
        default:
            oassert(!"unsupported type");
    };
};

void obj_AND(obj *op1, obj *op2, obj *result)
{
    oassert (op1->t==op2->t);

    switch (op1->t)
    {
        case OBJ_OCTABYTE:
            obj_octabyte2 (op1->u.ob & op2->u.ob, result);
            break;
        case OBJ_TETRABYTE:
            obj_tetrabyte2 (op1->u.tb & op2->u.tb, result);
            break;
        case OBJ_WYDE:
            obj_wyde2 (op1->u.w & op2->u.w, result);
            break;
        case OBJ_BYTE:
            obj_byte2 (op1->u.b & op2->u.b, result);
            break;
        default:
            oassert(!"unsupported type");
    };
};

void obj_OR(obj *op1, obj *op2, obj *result)
{
    oassert (op1->t==op2->t);

    switch (op1->t)
    {
        case OBJ_OCTABYTE:
            obj_octabyte2 (op1->u.ob | op2->u.ob, result);
            break;
        case OBJ_TETRABYTE:
            obj_tetrabyte2 (op1->u.tb | op2->u.tb, result);
            break;
        case OBJ_WYDE:
            obj_wyde2 (op1->u.w | op2->u.w, result);
            break;
        case OBJ_BYTE:
            obj_byte2 (op1->u.b | op2->u.b, result);
            break;
        default:
            oassert(!"unsupported type");
    };
};

int obj_compare(obj *op1, obj *op2)
{
    oassert (op1->t==op2->t);

    switch (op1->t)
    {
        case OBJ_OCTABYTE:
            {
                octabyte o1=obj_get_as_octabyte (op1);
                octabyte o2=obj_get_as_octabyte (op2);
                if (o1<o2)
                    return -1;
                if (o1>o2)
                    return 1;
                return 0;
            };

        case OBJ_TETRABYTE:
            {
                tetrabyte o1=obj_get_as_tetrabyte (op1);
                tetrabyte o2=obj_get_as_tetrabyte (op2);
                if (o1<o2)
                    return -1;
                if (o1>o2)
                    return 1;
                return 0;
            };

        case OBJ_WYDE:
            {
                wyde o1=obj_get_as_wyde (op1);
                wyde o2=obj_get_as_wyde (op2);
                if (o1<o2)
                    return -1;
                if (o1>o2)
                    return 1;
                return 0;
            };

        case OBJ_BYTE:
            {
                byte o1=obj_get_as_byte (op1);
                byte o2=obj_get_as_byte (op2);
                if (o1<o2)
                    return -1;
                if (o1>o2)
                    return 1;
                return 0;
            };

        default:
            oassert(!"unsupported type");
            fatal_error();
    };
};

int get_most_significant_bit(obj *i)
{
    switch (i->t)
    {
        case OBJ_OCTABYTE:
            return (obj_get_as_octabyte(i) & 0x8000000000000000) ? 1 : 0;
        case OBJ_TETRABYTE:
            return (obj_get_as_tetrabyte(i) & 0x80000000) ? 1 : 0;
        case OBJ_WYDE:
            return (obj_get_as_wyde(i) & 0x8000) ? 1 : 0;
        case OBJ_BYTE:
            return (obj_get_as_byte(i) & 0x80) ? 1 : 0;
        default:
            oassert(!"unsupported type");
            fatal_error();
    };
};

void obj_AND_with(obj* op1, byte op2)
{
    switch (op1->t)
    {
        case OBJ_OCTABYTE:
            op1->u.ob &= (octabyte)op2;
            break;
        case OBJ_TETRABYTE:
            op1->u.tb &= (tetrabyte)op2;
            break;
        case OBJ_WYDE:
            op1->u.w &= (wyde)op2;
            break;
        case OBJ_BYTE:
            op1->u.b &= op2;
            break;
        default:
            oassert(!"unsupported type");
    };
};

void obj2_sign_extended_shift_right (obj *op1, byte op2, obj *out)
{
    switch (op1->t)
    {
        case OBJ_OCTABYTE:
            obj_octabyte2 ((uint64_t)(((int64_t)obj_get_as_octabyte (op1)) >> op2), out);
            break;
        case OBJ_TETRABYTE:
            obj_tetrabyte2 ((uint32_t)(((int32_t)obj_get_as_tetrabyte (op1)) >> op2), out);
            break;
        case OBJ_WYDE:
            obj_wyde2 ((uint16_t)(((int16_t)obj_get_as_wyde (op1)) >> op2), out);
            break;
        case OBJ_BYTE:
            obj_byte2 ((uint8_t)(((int8_t)obj_get_as_byte (op1)) >> op2), out);
            break;
        default:
            oassert(0);
            break;
    };
};
                    
void obj_zero_extend (obj *in, enum obj_type out_type, obj* out)
{
    REG tmp=zero_extend_to_REG(in);

    switch (out_type)
    {
        case OBJ_WYDE:
            obj_wyde2 (tmp, out);
            break;
        case OBJ_TETRABYTE:
            obj_tetrabyte2 (tmp, out);
            break;
        case OBJ_OCTABYTE:
            obj_octabyte2 (tmp, out);
            break;
        default:
            oassert (!"other types are not yet supported");
    };
};

void obj_sign_extend (obj *in, enum obj_type out_type, obj* out)
{
    switch (out_type)
    {
        case OBJ_WYDE:
            oassert (in->t==OBJ_BYTE);
            obj_wyde2((uint16_t)(int16_t)(int8_t)obj_get_as_byte(in), out);
            break;

        case OBJ_TETRABYTE:
            if (in->t==OBJ_BYTE)
                obj_tetrabyte2((uint32_t)(int32_t)(int8_t)obj_get_as_byte(in), out);
            else if (in->t==OBJ_WYDE)
                obj_tetrabyte2((uint32_t)(int32_t)(int16_t)obj_get_as_wyde(in), out);
            else
            {
                oassert(0);
            };
            break;

        case OBJ_OCTABYTE:
            if (in->t==OBJ_BYTE)
                obj_octabyte2((uint64_t)(int64_t)(int8_t)obj_get_as_byte(in), out);
            else if (in->t==OBJ_WYDE)
                obj_octabyte2((uint64_t)(int64_t)(int16_t)obj_get_as_wyde(in), out);
            else if (in->t==OBJ_TETRABYTE)
                obj_octabyte2((uint64_t)(int64_t)(int32_t)obj_get_as_tetrabyte(in), out);
            else
            {
                oassert(0);
            };
            break;

        default:
            oassert (!"other types are not yet supported");
    };
};

/* vim: set expandtab ts=4 sw=4 : */

