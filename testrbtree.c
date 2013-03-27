#include "rbtree.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h> /* rand() */
#include <stdint.h>

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

int main() 
{
    int i;
    rbtree t = rbtree_create();

    printf ("enumerate:\n");
    rbtree_enumerate(t, visitor);

    rbtree_insert(t, (void*)12345, (void*)"value1", compare_uint32_t);
    assert(strcmp (rbtree_lookup(t, (void*)12345, compare_uint32_t), "value1")==0);

    rbtree_insert(t, (void*)12346, (void*)"value2", compare_uint32_t);
    
    printf ("enumerate:\n");
    rbtree_enumerate(t, visitor);
    
    rbtree_delete(t, (void*)12345, compare_uint32_t);
    
    rbtree_insert(t, (void*)12347, (void*)"value3", compare_uint32_t);
    
    printf ("enumerate:\n");
    rbtree_enumerate(t, visitor);
    return 0;
}
