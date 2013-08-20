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

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "datatypes.h"

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
    bool use_dmalloc/*:8*/;
    const char *struct_name;

    compare_func cmp_func;

    // reinventing C++?
    // callback: value comparing function
    // callback: key/value deallocating function
    // callback: key/value copying function
} rbtree;
//#pragma pack(pop)

rbtree *rbtree_create(bool use_dmalloc, const char *struct_name, compare_func compare);
void rbtree_clear(rbtree* t);

bool rbtree_is_key_present(rbtree *tree, void* key);

// returning VALUE but not node! if there is an entry where value==NULL, NULL will return
// do not use it for key presence check!
void* rbtree_lookup(rbtree* t, void* key);

// extended version
// output_(lower|upper)_bound_(k|v) are set if key not found. 
// they may be NULL if results are not needed
void* rbtree_lookup2(rbtree* t, void* key, 
        void** out_prev_k, void** out_prev_v,
        void** out_next_k, void** out_next_v);


void rbtree_insert(rbtree* t, void* key, void* value);
void rbtree_delete(rbtree* t, void* key); // FIXME: this fn do lookup first. there should be function for deleting by ptr to value!
void rbtree_deinit(rbtree* t);

void rbtree_foreach(rbtree* t, void (*visitor_kv)(void*, void*), 
        void (*visitor_k)(void*), void (*visitor_v)(void*));

int compare_size_t(void* leftp, void* rightp);
int compare_int(void* leftp, void* rightp);
int compare_tetrabytes(void* leftp, void* rightp);

struct rbtree_node_t *rbtree_minimum(rbtree* t); // will return NULL for empty tree
struct rbtree_node_t *rbtree_maximum(rbtree* t); // will return NULL for empty tree
struct rbtree_node_t *rbtree_succ(struct rbtree_node_t* x);
struct rbtree_node_t *rbtree_pred(struct rbtree_node_t* x);

void rbtree_copy (rbtree* t, rbtree* new_t, void* (*key_copier)(void*), void* (*value_copier)(void*));

bool rbtree_empty (rbtree* t);
unsigned rbtree_count(rbtree *t);
// sorted. return as array. caller should allocate space
void rbtree_return_all_keys (rbtree *t, void **out);

#ifdef  __cplusplus
}
#endif

/* vim: set expandtab ts=4 sw=4 : */
