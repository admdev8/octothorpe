#include <stdio.h>
#include <assert.h>
#include <stdlib.h> /* rand() */
#include <stdint.h>

#include "dmalloc.h"
#include "rbtree.h"

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

void tst_succ_pred(rbtree* t)
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
    rbtree* t = rbtree_create(true, "test", compare_uint32_t);
    rbtree* new_t = rbtree_create(true, "new_test", compare_uint32_t);
    rbtree* t2;
    void *key_prev, *value_prev, *key_next, *value_next;

    printf ("enumerate:\n");
    rbtree_foreach(t, visitor, NULL, NULL);

    rbtree_insert(t, (void*)12345, (void*)"value1");
    assert(strcmp (rbtree_lookup(t, (void*)12345), "value1")==0);

    rbtree_insert(t, (void*)12346, (void*)"value2");

    printf ("enumerate:\n");
    rbtree_foreach(t, visitor, NULL, NULL);

    rbtree_delete(t, (void*)12345);

    rbtree_insert(t, (void*)12347, (void*)"value3");
    rbtree_insert(t, (void*)666, (void*)"666");
    rbtree_insert(t, (void*)0, (void*)"zero!");

    printf ("enumerate:\n");
    rbtree_foreach(t, visitor, NULL, NULL);
    printf ("minimum: %d\n", rbtree_minimum(t)->key);
    printf ("maximum: %d\n", rbtree_maximum(t)->key);

    tst_succ_pred(t);

    rbtree_copy (t, new_t, key_copier, value_copier);
    printf ("enumerate new_t:\n");
    rbtree_foreach(t, visitor, NULL, NULL);

    printf ("freeing:\n");
    rbtree_foreach(new_t, NULL, NULL, dfree);

    rbtree_deinit(t);
    rbtree_deinit(new_t);

    // test 2
    printf ("test 2 (lower/upper bounds)\n");

    t2=rbtree_create(true, "test", compare_uint32_t);

    //rbtree_insert (t2, (void*)0, "");
    rbtree_insert (t2, (void*)50, "value 50");
    rbtree_insert (t2, (void*)99, "value 99");
    rbtree_insert (t2, (void*)101, "value 101");
    rbtree_insert (t2, (void*)500, "value 500");
    rbtree_insert (t2, (void*)12200, "value 12200");
    rbtree_insert (t2, (void*)12301, "value 12301");
    rbtree_insert (t2, (void*)50000, "value 50000");
    rbtree_insert (t2, (void*)60000, "value 60000");
    printf ("enumerate t2:\n");
    rbtree_foreach(t2, visitor, NULL, NULL);

    rbtree_lookup2(t2, (void*)10, &key_prev, &value_prev, &key_next, &value_next);
    printf ("while looking for 10, key_prev=%d, value_prev=%s, key_next=%d, value_next=%s\n", key_prev, value_prev, key_next, value_next);

    rbtree_lookup2(t2, (void*)100, &key_prev, &value_prev, &key_next, &value_next);
    printf ("while looking for 100, key_prev=%d, value_prev=%s, key_next=%d, value_next=%s\n", key_prev, value_prev, key_next, value_next);

    rbtree_lookup2(t2, (void*)101, &key_prev, &value_prev, &key_next, &value_next);
    printf ("while looking for 101, key_prev=%d, value_prev=%s, key_next=%d, value_next=%s\n", key_prev, value_prev, key_next, value_next);

    rbtree_lookup2(t2, (void*)12300, &key_prev, &value_prev, &key_next, &value_next);
    printf ("while looking for 12300, key_prev=%d, value_prev=%s, key_next=%d, value_next=%s\n", key_prev, value_prev, key_next, value_next);

    rbtree_lookup2(t2, (void*)99999, &key_prev, &value_prev, &key_next, &value_next);
    printf ("while looking for 99999, key_prev=%d, value_prev=%s, key_next=%d, value_next=%s\n", key_prev, value_prev, key_next, value_next);

    rbtree_deinit(t2);

    dump_unfreed_blocks();

    return 0;
}
