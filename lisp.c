#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "dmalloc.h"

#include "lisp.h"

#define FILE_OUT stdout
//#define FILE_OUT stderr

obj* obj_int (int i)
{
    obj* rt;
    //fprintf (FILE_OUT, "obj_int(%d)", i);
    rt=DCALLOC (obj, 1, "obj");
    rt->t=OBJ_INTEGER;
    rt->u.i=i;
    return rt;
};

obj* obj_double (double d)
{
    obj* rt;
    rt=DCALLOC (obj, 1, "obj");
    rt->t=OBJ_DOUBLE;
    rt->u.d=d;
    return rt;
};

obj* obj_int_n_times (int i, int t)
{
    if (t==0)
        return NULL;
    return cons (obj_int(i), obj_int_n_times(i, t-1));
};

obj* obj_cstring (const char *s)
{
    obj* rt=DCALLOC (obj, 1, "obj");
    rt->t=OBJ_CSTRING;
    rt->u.s=strdup (s);
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

bool obj_is_list(obj *o)
{
    assert (o);

    if (o->t!=OBJ_CONS)
        return false;

    if (o->u.c->head->t==OBJ_CONS)
        return false;

    if (o->u.c->tail==NULL)
        return true;

    if (o->u.c->tail->t==OBJ_CONS)
        return obj_is_list (o->u.c->tail);
    else // tail is not cons and not NULL
        return false;
};

void obj_dump_as_list(obj *o)
{
    obj* c;
    assert (obj_is_list(o));

    // treat it as list!

    printf ("(");
    for (c=o; ; c=c->u.c->tail)
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
    //fprintf (FILE_OUT, "obj_dump(0x%p)\n", o);
    if(o==NULL)
    {
        fprintf (FILE_OUT, "NULL");
        return;
    };

    if (obj_is_list(o))
    {
        obj_dump_as_list(o);
        return;
    };

    switch (o->t)
    {
        case OBJ_INTEGER:
            fprintf (FILE_OUT, "0x%x", o->u.i);
            break;
        case OBJ_DOUBLE:
            fprintf (FILE_OUT, "%f", o->u.d);
            break;
        case OBJ_CSTRING:
            fprintf (FILE_OUT, "\"%s\"", o->u.s);
            break;
        case OBJ_CONS:
            assert (o->u.c);
            fprintf (FILE_OUT, "(");
            obj_dump(o->u.c->head);
            fprintf(FILE_OUT, " ");
            obj_dump(o->u.c->tail);
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

static obj* find_last_cons(obj *l)
{
    assert (l->t==OBJ_CONS);
    if (l->u.c->tail==NULL)
        return l;
    else
        return find_last_cons(l->u.c->tail);
};

obj* nconc (obj *l1, obj *l2)
{
    // find last element of l1. it should be NULL.
    obj *last=find_last_cons(l1);
    assert(last->t==OBJ_CONS);
    last->u.c->tail=l2;
    return l1;
};

void obj_free(obj* o)
{
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
        default:
            break;
    };
    DFREE(o);
};

