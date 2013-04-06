// reworked by Dennis Yurichev

/* Copyright (c) 2013 the authors listed at the following URL, and/or
the authors of referenced articles or incorporated external code:
http://en.literateprograms.org/Red-black_tree_(C)?action=history&offset=20120524204657

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

#include "rbtree.h"
#include <assert.h>
#include <stdlib.h>
#include "dmalloc.h"

#if 0
#define VERIFY_RBTREE
#endif

typedef rbtree_node node;
typedef enum rbtree_node_color color;

static rbtree_node* grandparent(rbtree_node* n);
static rbtree_node* sibling(rbtree_node* n);
static rbtree_node* uncle(rbtree_node* n);
static void verify_properties(rbtree* t);
static void verify_property_1(node* root);
static void verify_property_2(node* root);
static color node_color(node* n);
static void verify_property_4(node* root);
static void verify_property_5(node* root);
static void verify_property_5_helper(node* n, int black_count, int* black_count_path);

static rbtree_node* new_node(rbtree* t, void* key, void* value, color node_color, node* left, node* right);
static rbtree_node* lookup_node(rbtree* t, void* key);
static void rotate_left(rbtree* t, node* n);
static void rotate_right(rbtree* t, node* n);

static void replace_node(rbtree* t, node* oldn, node* newn);
static void insert_case1(rbtree* t, node* n);
static void insert_case2(rbtree* t, node* n);
static void insert_case3(rbtree* t, node* n);
static void insert_case4(rbtree* t, node* n);
static void insert_case5(rbtree* t, node* n);
static void delete_case1(rbtree* t, node* n);
static void delete_case2(rbtree* t, node* n);
static void delete_case3(rbtree* t, node* n);
static void delete_case4(rbtree* t, node* n);
static void delete_case5(rbtree* t, node* n);
static void delete_case6(rbtree* t, node* n);

rbtree_node* grandparent(rbtree_node* n) 
{
    assert (n != NULL);
    assert (n->parent != NULL); /* Not the root node */
    assert (n->parent->parent != NULL); /* Not child of root */
    return n->parent->parent;
}
rbtree_node* sibling(rbtree_node* n) 
{
    assert (n != NULL);
    assert (n->parent != NULL); /* Root node has no sibling */
    if (n == n->parent->left)
        return n->parent->right;
    else
        return n->parent->left;
}
rbtree_node* uncle(rbtree_node* n) 
{
    assert (n != NULL);
    assert (n->parent != NULL); /* Root node has no uncle */
    assert (n->parent->parent != NULL); /* Children of root have no uncle */
    return sibling(n->parent);
}
void verify_properties(rbtree* t) 
{
#ifdef VERIFY_RBTREE
    verify_property_1(t->root);
    verify_property_2(t->root);
    /* Property 3 is implicit */
    verify_property_4(t->root);
    verify_property_5(t->root);
#endif
}
void verify_property_1(node* n) {
    assert(node_color(n) == RED || node_color(n) == BLACK);
    if (n == NULL) return;
    verify_property_1(n->left);
    verify_property_1(n->right);
}
void verify_property_2(node* root) 
{
    assert(node_color(root) == BLACK);
}
color node_color(node* n) 
{
    return n == NULL ? BLACK : n->color;
}
void verify_property_4(node* n) 
{
    if (node_color(n) == RED) 
    {
        assert (node_color(n->left)   == BLACK);
        assert (node_color(n->right)  == BLACK);
        assert (node_color(n->parent) == BLACK);
    }
    if (n == NULL) return;
    verify_property_4(n->left);
    verify_property_4(n->right);
}
void verify_property_5(node* root) 
{
    int black_count_path = -1;
    verify_property_5_helper(root, 0, &black_count_path);
}

void verify_property_5_helper(node* n, int black_count, int* path_black_count) 
{
    if (node_color(n) == BLACK) {
        black_count++;
    }
    if (n == NULL) {
        if (*path_black_count == -1) {
            *path_black_count = black_count;
        } else {
            assert (black_count == *path_black_count);
        }
        return;
    }
    verify_property_5_helper(n->left,  black_count, path_black_count);
    verify_property_5_helper(n->right, black_count, path_black_count);
}

static struct rbtree_node_t* rbtree_maximum_helper(struct rbtree_node_t* n)
{
    if (n->right)
        return rbtree_maximum_helper(n->right);
    else
        return n;
};

rbtree* rbtree_create(BOOL use_dmalloc, const char *struct_name, compare_func compare) 
{
    rbtree* t;

    if (use_dmalloc)
        t = DMALLOC(sizeof(struct rbtree_t), struct_name);
    else
        t = malloc(sizeof(struct rbtree_t));
    t->root = NULL;
    verify_properties(t);
    t->use_dmalloc=use_dmalloc;
    t->struct_name=struct_name;
    t->cmp_func=compare;
    return t;
}

void rbtree_deinit(rbtree* t)
{
    rbtree_clear(t);

    if (t->use_dmalloc)
        DFREE(t);
    else
        free(t);
};

static void rbtree_clear_helper(rbtree* t, struct rbtree_node_t *n)
{
    if (n->right)
        rbtree_clear_helper(t, n->right);
    if (n->left)
        rbtree_clear_helper(t, n->left);
    if (t->use_dmalloc)
        DFREE (n);
    else
        free(n);
};

void rbtree_clear(rbtree* t)
{
    if (rbtree_empty(t))
        return;
    rbtree_clear_helper(t, t->root);
};

rbtree_node* new_node(rbtree* t, void* key, void* value, color node_color, node* left, node* right) 
{
    rbtree_node* result;
    if (t->use_dmalloc)
        result = DMALLOC(sizeof(struct rbtree_node_t), t->struct_name);
    else
        result = malloc(sizeof(struct rbtree_node_t));
    result->key = key;
    result->value = value;
    result->color = node_color;
    result->left = left;
    result->right = right;
    if (left  != NULL)  left->parent = result;
    if (right != NULL) right->parent = result;
    result->parent = NULL;
    return result;
};

node* lookup_node(rbtree* t, void* key) 
{
    rbtree_node* n = t->root;
    while (n != NULL) 
    {
        int comp_result = t->cmp_func(key, n->key);
        if (comp_result == 0) 
        {
            return n;
        } 
        else if (comp_result < 0) 
        {
            n = n->left;
        } 
        else 
        {
            assert(comp_result > 0);
            n = n->right;
        }
    }
    return n;
}
void* rbtree_lookup(rbtree* t, void* key) 
{
    rbtree_node* n = lookup_node(t, key);
    return n == NULL ? NULL : n->value;
}
void rotate_left(rbtree* t, node* n) 
{
    rbtree_node* r = n->right;
    replace_node(t, n, r);
    n->right = r->left;
    if (r->left != NULL) {
        r->left->parent = n;
    }
    r->left = n;
    n->parent = r;
}

void rotate_right(rbtree* t, node* n) 
{
    rbtree_node* L = n->left;
    replace_node(t, n, L);
    n->left = L->right;
    if (L->right != NULL) 
    {
        L->right->parent = n;
    }
    L->right = n;
    n->parent = L;
}
void replace_node(rbtree* t, node* oldn, node* newn) 
{
    if (oldn->parent == NULL) 
    {
        t->root = newn;
    } 
    else 
    {
        if (oldn == oldn->parent->left)
            oldn->parent->left = newn;
        else
            oldn->parent->right = newn;
    }
    if (newn != NULL) 
    {
        newn->parent = oldn->parent;
    }
}
void rbtree_insert(rbtree* t, void* key, void* value) 
{
    rbtree_node* inserted_node = new_node(t, key, value, RED, NULL, NULL);
    if (t->root == NULL) 
    {
        t->root = inserted_node;
    } 
    else 
    {
        rbtree_node* n = t->root;
        while (1) 
        {
            int comp_result = t->cmp_func(key, n->key);
            if (comp_result == 0) 
            {
                n->value = value;
                /* inserted_node isn't going to be used, don't leak it */
                if (t->use_dmalloc)
                    dfree (inserted_node);
                else
                    free (inserted_node);
                return;
            } 
            else if (comp_result < 0) 
            {
                if (n->left == NULL) 
                {
                    n->left = inserted_node;
                    break;
                } 
                else 
                {
                    n = n->left;
                }
            } 
            else 
            {
                assert (comp_result > 0);
                if (n->right == NULL) 
                {
                    n->right = inserted_node;
                    break;
                } 
                else 
                {
                    n = n->right;
                }
            }
        }
        inserted_node->parent = n;
    }
    insert_case1(t, inserted_node);
    verify_properties(t);
}
void insert_case1(rbtree* t, node* n) 
{
    if (n->parent == NULL)
        n->color = BLACK;
    else
        insert_case2(t, n);
}
void insert_case2(rbtree* t, node* n) 
{
    if (node_color(n->parent) == BLACK)
        return; /* Tree is still valid */
    else
        insert_case3(t, n);
}
void insert_case3(rbtree* t, node* n) 
{
    if (node_color(uncle(n)) == RED) 
    {
        n->parent->color = BLACK;
        uncle(n)->color = BLACK;
        grandparent(n)->color = RED;
        insert_case1(t, grandparent(n));
    } 
    else 
    {
        insert_case4(t, n);
    }
}
void insert_case4(rbtree* t, node* n) 
{
    if (n == n->parent->right && n->parent == grandparent(n)->left) 
    {
        rotate_left(t, n->parent);
        n = n->left;
    } 
    else if (n == n->parent->left && n->parent == grandparent(n)->right) 
    {
        rotate_right(t, n->parent);
        n = n->right;
    }
    insert_case5(t, n);
}
void insert_case5(rbtree* t, node* n) 
{
    n->parent->color = BLACK;
    grandparent(n)->color = RED;
    if (n == n->parent->left && n->parent == grandparent(n)->left) 
    {
        rotate_right(t, grandparent(n));
    } 
    else 
    {
        assert (n == n->parent->right && n->parent == grandparent(n)->right);
        rotate_left(t, grandparent(n));
    }
}

void rbtree_delete(rbtree* t, void* key) 
{
    rbtree_node* child;
    rbtree_node* n = lookup_node(t, key);
    if (n == NULL) 
        return;  /* Key not found, do nothing */
    if (n->left != NULL && n->right != NULL) 
    {
        /* Copy key/value from predecessor and then delete it instead */
        rbtree_node* pred = rbtree_maximum_helper(n->left);
        n->key   = pred->key;
        n->value = pred->value;
        n = pred;
    }

    assert(n->left == NULL || n->right == NULL);
    child = n->right == NULL ? n->left  : n->right;
    if (node_color(n) == BLACK) 
    {
        n->color = node_color(child);
        delete_case1(t, n);
    }
    replace_node(t, n, child);
    if (n->parent == NULL && child != NULL)
        child->color = BLACK;
    if (t->use_dmalloc)
        dfree(n);
    else
        free(n);

    verify_properties(t);
}
/*
 * TODO remove this
 *
static rbtree_node* maximum_node(node* n) 
{
    assert (n != NULL);
    while (n->right != NULL) 
    {
        n = n->right;
    }
    return n;
}
*/
void delete_case1(rbtree* t, node* n) 
{
    if (n->parent == NULL)
        return;
    else
        delete_case2(t, n);
}
void delete_case2(rbtree* t, node* n) 
{
    if (node_color(sibling(n)) == RED) 
    {
        n->parent->color = RED;
        sibling(n)->color = BLACK;
        if (n == n->parent->left)
            rotate_left(t, n->parent);
        else
            rotate_right(t, n->parent);
    }
    delete_case3(t, n);
}
void delete_case3(rbtree* t, node* n) 
{
    if (node_color(n->parent) == BLACK &&
        node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == BLACK &&
        node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        delete_case1(t, n->parent);
    }
    else
        delete_case4(t, n);
}
void delete_case4(rbtree* t, node* n) 
{
    if (node_color(n->parent) == RED &&
        node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == BLACK &&
        node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        n->parent->color = BLACK;
    }
    else
        delete_case5(t, n);
}
void delete_case5(rbtree* t, node* n) 
{
    if (n == n->parent->left &&
        node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == RED &&
        node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->left->color = BLACK;
        rotate_right(t, sibling(n));
    }
    else if (n == n->parent->right &&
             node_color(sibling(n)) == BLACK &&
             node_color(sibling(n)->right) == RED &&
             node_color(sibling(n)->left) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->right->color = BLACK;
        rotate_left(t, sibling(n));
    }
    delete_case6(t, n);
}
void delete_case6(rbtree* t, node* n) 
{
    sibling(n)->color = node_color(n->parent);
    n->parent->color = BLACK;
    if (n == n->parent->left) 
    {
        assert (node_color(sibling(n)->right) == RED);
        sibling(n)->right->color = BLACK;
        rotate_left(t, n->parent);
    }
    else
    {
        assert (node_color(sibling(n)->left) == RED);
        sibling(n)->left->color = BLACK;
        rotate_right(t, n->parent);
    }
}

static void rbtree_foreach_helper(rbtree_node* n, void (*visitor_kv)(void*, void*),
        void (*visitor_k)(void*), void (*visitor_v)(void*))
{
    if (n->left!=NULL)
        rbtree_foreach_helper(n->left, visitor_kv, visitor_k, visitor_v);
    
    if (visitor_kv)
        visitor_kv (n->key, n->value);

    if (visitor_k)
        visitor_k (n->key);

    if (visitor_v)
        visitor_v (n->value);
    
    if (n->right!=NULL)
        rbtree_foreach_helper(n->right, visitor_kv, visitor_k, visitor_v);
};

void rbtree_foreach(rbtree* t, void (*visitor_kv)(void*, void*), 
        void (*visitor_k)(void*), void (*visitor_v)(void*)){
    if (t && t->root)
        rbtree_foreach_helper (t->root, visitor_kv, visitor_k, visitor_v);
};

int compare_size_t(void* leftp, void* rightp)
{
    size_t left = (size_t)leftp;
    size_t right = (size_t)rightp;
    if (left < right)
        return -1;
    else if (left > right)
        return 1;
    else 
    {
        assert (left == right);
        return 0;
    };
};

/*
void free_value_by_DFREE (void *k, void *v)
{
    DFREE(v);
};
*/

static struct rbtree_node_t* rbtree_minimum_helper(struct rbtree_node_t* n)
{
    if (n->left)
        return rbtree_minimum_helper(n->left);
    else
        return n;
};

struct rbtree_node_t* rbtree_minimum(rbtree* t)
{
    if (rbtree_empty(t))
        return NULL;
    return rbtree_minimum_helper(t->root);
};

struct rbtree_node_t* rbtree_maximum(rbtree* t)
{
    if (rbtree_empty(t))
        return NULL;
    return rbtree_maximum_helper(t->root);
};

struct rbtree_node_t* rbtree_succ(struct rbtree_node_t* x)
{
    // algorithm copied from "Introduction to Algorithms" book
    
    struct rbtree_node_t *y;

    if (x->right)
        return rbtree_minimum_helper(x->right);

    y=x->parent;
    while (y!=NULL && x==y->right)
    {
        x=y;
        y=y->parent;
    };
    return y;
};

struct rbtree_node_t* rbtree_pred(struct rbtree_node_t* x)
{
    // algorithm copied from "Introduction to Algorithms" book
    
    struct rbtree_node_t *y;

    if (x->left)
        return rbtree_maximum_helper(x->left);

    y=x->parent;
    while (y!=NULL && x==y->left)
    {
        x=y;
        y=y->parent;
    };
    return y;
};

void rbtree_copy (rbtree* t, rbtree* new_t, void* (*key_copier)(void*), void* (*value_copier)(void*))
{
    struct rbtree_node_t *i;

    // this can be faster, using rbtree_foreach()
    for (i=rbtree_minimum(t); i!=NULL; i=rbtree_succ(i))
    {
        void *new_k=(*key_copier)(i->key);
        void *new_v=(*value_copier)(i->value);
        rbtree_insert(new_t, new_k, new_v);
    };
};

BOOL rbtree_empty (rbtree* t)
{
    return t->root==NULL ? TRUE : FALSE;
};
