// reworked by Dennis Yurichev

/* Copyright (c) 2013 the authors listed at the following URL, and/or
the authors of referenced articles or incorporated external code:
http://en.literateprograms.org/Red-black_tree_(C)?action=history&offset=20120524204657

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Red-black_tree_(C)?oldid=18555
*/

#pragma once

#include "bool.h"

#ifdef  __cplusplus
extern "C" {
#endif

enum rbtree_node_color { RED, BLACK };

//#pragma pack(push)
//#pragma pack(1)
typedef struct rbtree_node_t 
{
    void* key;
    void* value;
    struct rbtree_node_t* left;
    struct rbtree_node_t* right;
    // int right_count_of_hits? so to optimize subsequent blocks in memory for cache optimization?
    struct rbtree_node_t* parent;
    enum rbtree_node_color color:8;
} rbtree_node;
//#pragma pack(pop)

typedef int (*compare_func)(void* left, void* right);

//#pragma pack(push)
//#pragma pack(1)
typedef struct rbtree_t
{
    rbtree_node *root;
    
    // use DMALLOC? if so, pass debug string
    BOOL use_dmalloc:8;
    const char *struct_name;

    compare_func cmp_func;

    // reinventing C++?
    // callback: value comparing function
    // callback: key/value deallocating function
    // callback: key/value copying function
} rbtree;
//#pragma pack(pop)

rbtree *rbtree_create(BOOL use_dmalloc, const char *struct_name, compare_func compare);
void rbtree_clear(rbtree* t);
void* rbtree_lookup(rbtree* t, void* key);
void rbtree_insert(rbtree* t, void* key, void* value);
void rbtree_delete(rbtree* t, void* key);
void rbtree_deinit(rbtree* t);

void rbtree_foreach(rbtree* t, void (*visitor_kv)(void*, void*), 
        void (*visitor_k)(void*), void (*visitor_v)(void*));

int compare_size_t(void* leftp, void* rightp);

// can be call_func_for_value().
//void free_value_by_DFREE (void *k, void *v);

struct rbtree_node_t *rbtree_minimum(rbtree* t); // will return NULL for empty tree
struct rbtree_node_t *rbtree_maximum(rbtree* t); // will return NULL for empty tree
struct rbtree_node_t *rbtree_succ(struct rbtree_node_t* x);
struct rbtree_node_t *rbtree_pred(struct rbtree_node_t* x);

void rbtree_copy (rbtree* t, rbtree* new_t, void* (*key_copier)(void*), void* (*value_copier)(void*));

BOOL rbtree_empty (rbtree* t);

#ifdef  __cplusplus
}
#endif

