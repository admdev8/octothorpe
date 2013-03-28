// slightly reworked by Dennis Yurichev

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

typedef struct rbtree_node_t {
    void* key;
    void* value;
    struct rbtree_node_t* left;
    struct rbtree_node_t* right;
    struct rbtree_node_t* parent;
    enum rbtree_node_color color;
} *rbtree_node;

typedef int (*compare_func)(void* left, void* right);

typedef struct rbtree_t {
    rbtree_node root;
    
    // use DMALLOC? if so, pass debug string
    BOOL use_dmalloc;
    const char *struct_name;

    compare_func cmp_func;
} *rbtree;

rbtree rbtree_create(BOOL use_dmalloc, const char *struct_name, compare_func compare);
void rbtree_clear();
void* rbtree_lookup(rbtree t, void* key);
void rbtree_insert(rbtree t, void* key, void* value);
void rbtree_delete(rbtree t, void* key);

void rbtree_walk(rbtree t, void (*visitor)(void*, void*));
int compare_size_t(void* leftp, void* rightp);

#ifdef  __cplusplus
}
#endif

