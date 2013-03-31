#include "rbtree.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h> /* rand() */
#include <stdint.h>
#include "dmalloc.h"

int compare_uint32_t(void* leftp, void* rightp) 
{
    uint32_t left = (uint32_t)leftp;
    uint32_t right = (uint32_t)rightp;
    if (left < right) 
        return -1;
    else if (left > right)
        return 1;
    else {
        assert (left == right);
        return 0;
    }
}

void visitor(void* k, void* v)
{
    printf ("key=%d value=%s\n", (uint32_t)k, v);
};

void tst_succ_pred(rbtree t)
{
    struct rbtree_node_t *i;

    printf ("ascending order:\n");
    for (i=rbtree_minimum(t); i!=NULL; i=rbtree_succ(i))
        printf ("%d\n", i->key);

    printf ("descending order:\n");
    for (i=rbtree_maximum(t); i!=NULL; i=rbtree_pred(i))
        printf ("%d\n", i->key);
};

void* key_copier(void *i)
{
    return i;
};

void* value_copier(void *v)
{
    return DMEMDUP (v, strlen(v)+1, "string");
};

int main() 
{
    int i;
    rbtree t = rbtree_create(TRUE, "test", compare_uint32_t);
    rbtree new_t = rbtree_create(TRUE, "new_test", compare_uint32_t);

    printf ("enumerate:\n");
    rbtree_foreach(t, visitor);

    rbtree_insert(t, (void*)12345, (void*)"value1");
    assert(strcmp (rbtree_lookup(t, (void*)12345), "value1")==0);

    rbtree_insert(t, (void*)12346, (void*)"value2");

    printf ("enumerate:\n");
    rbtree_foreach(t, visitor);

    rbtree_delete(t, (void*)12345);

    rbtree_insert(t, (void*)12347, (void*)"value3");
    rbtree_insert(t, (void*)666, (void*)"666");
    rbtree_insert(t, (void*)0, (void*)"zero!");

    printf ("enumerate:\n");
    rbtree_foreach(t, visitor);
    printf ("minimum: %d\n", rbtree_minimum(t)->key);
    printf ("maximum: %d\n", rbtree_maximum(t)->key);

    tst_succ_pred(t);

    rbtree_copy (t, new_t, key_copier, value_copier);
    printf ("enumerate new_t:\n");
    rbtree_foreach(t, visitor);

    printf ("freeing:\n");
    rbtree_foreach(new_t, free_value_by_DFREE);

    rbtree_deinit(t);
    rbtree_deinit(new_t);

    dump_unfreed_blocks();

    return 0;
}
