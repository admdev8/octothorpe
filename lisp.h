#pragma once

// "Any sufficiently complicated C or Fortran program contains an ad hoc, 
// informally-specified, bug-ridden, slow implementation of half of Common Lisp." 
// (Greenspun's tenth rule of programming)

enum obj_type
{
    OBJ_INTEGER,
    OBJ_DOUBLE,
    OBJ_CSTRING,
    OBJ_CONS,
    OBJ_OPAQUE
};

struct _cons_cell;

typedef struct _obj_opaque
{
    void* ptr;
    void (*dumper_fn) (void*); // may be NULL
    void (*free_fn) (void*); // may be NULL
} obj_opaque;

typedef struct _obj
{
    enum obj_type t;
    union
    {
        int i; // OBJ_INTEGER
        double d; // OBJ_DOUBLE
        char *s; // OBJ_CSTRING
        struct _cons_cell *c; // OBJ_CONS
        struct _obj_opaque *o; // OBJ_OPAQUE
    } u;
} obj;

typedef struct _cons_cell
{
    obj *head; // AKA car
    obj *tail; // AKA cdr
} cons_cell;

obj* obj_int (int i);
obj* obj_double (double i);
obj* obj_int_n_times (int i, int t);
obj* obj_cstring (const char *s);
obj* cons (obj* head, obj* tail);
obj* create_obj_opaque(void* ptr, void (*dumper_fn) (void*), void (*free_fn) (void*));
bool obj_is_list(obj *o);
void obj_dump_as_list(obj *o);
void obj_dump(obj *o);
obj *nconc (obj *l1, obj *l2);
void obj_free(obj* o);
bool obj_is_cons(obj* o);
obj* car(obj* o);
obj* cdr(obj* o);
bool obj_is_opaque(obj* o);
void* obj_unpack_opaque(obj* o);


