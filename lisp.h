#pragma once

#include <stdbool.h>
#include "datatypes.h"

// "Any sufficiently complicated C or Fortran program contains an ad hoc, 
// informally-specified, bug-ridden, slow implementation of half of Common Lisp." 
// (Greenspun's tenth rule of programming)

enum obj_type
{
    OBJ_BYTE,
    OBJ_WYDE,
    OBJ_TETRABYTE,
    OBJ_OCTABYTE,
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
        byte b; // OBJ_BYTE
        wyde w; // OBJ_WYDE
        tetrabyte tb; // OBJ_TETRABYTE
        octabyte ob; // OBJ_OCTABYTE
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

obj* lisp_LAST(obj *l);
obj* obj_byte (byte i);
obj* obj_wyde (wyde i);
obj* obj_tetrabyte (tetrabyte i);
obj* obj_octabyte (octabyte i);
obj* obj_REG (REG i);
obj* obj_double (double i);
obj* obj_wyde_n_times (wyde i, int t);
obj* obj_tetrabyte_n_times (tetrabyte i, int t);
obj* obj_cstring (const char *s);
obj* cons (obj* head, obj* tail);
obj* create_obj_opaque(void* ptr, void (*dumper_fn) (void*), void (*free_fn) (void*));
bool LISTP(obj *o);
void obj_dump_as_list(obj *o);
void obj_dump(obj *o);

// l1 may be NULL, it's OK. l1 and l2 should be lists!
obj *NCONC (obj *l1, obj *l2);
void obj_free(obj* o);
void obj_free_conses_of_list(obj* o);
bool CONSP(obj* o);
obj* car(obj* o);
obj* cdr(obj* o);
bool obj_is_opaque(obj* o);
void* obj_unpack_opaque(obj* o);
tetrabyte obj_get_as_tetrabyte(obj* o);
octabyte obj_get_as_octabyte(obj* o);
REG obj_get_as_REG(obj* o);
char* obj_get_as_cstring(obj* o);
// terminated with NULL
obj* create_list(obj* o, ...);
