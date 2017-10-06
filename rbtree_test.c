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

#include <stdio.h>
#include <string.h> // strcmp
#include "oassert.h"
#include <stdlib.h> /* rand() */
#include <stdint.h>

#include "datatypes.h"
#include "dmalloc.h"
#include "rbtree.h"
#include "fmt_utils.h"

int compare_tetra(void* leftp, void* rightp) 
{
    tetra left = (tetra)leftp;
    tetra right = (tetra)rightp;
    if (left < right) 
        return -1;
    else if (left > right)
        return 1;
    else {
        oassert (left == right);
        return 0;
    }
}

void visitor(void* k, void* v)
{
    printf ("key=%d value=%s\n", (tetra)k, (char*)v);
};

void test_return_all_keys(rbtree *t)
{
    printf ("%s() begin\n", __func__);
    unsigned cnt=rbtree_count(t);
    REG *buf=DMALLOC(REG, cnt, "REG");
    rbtree_return_all_keys (t, buf);
    for (unsigned i=0; i<cnt; i++)
        printf ("%d\n", buf[i]);
    DFREE(buf);
    printf ("%s() end\n", __func__);
};

void tst_succ_pred(rbtree* t)
{
    printf ("ascending order:\n");
    for (rbtree_node *i=rbtree_minimum(t); i; i=rbtree_succ(i))
        printf ("%d\n", (int)i->key);

    printf ("descending order:\n");
    for (rbtree_node *i=rbtree_maximum(t); i; i=rbtree_pred(i))
        printf ("%d\n", (int)i->key);
};

void* key_copier(void *i)                             
{
    return i;
};

void* value_copier(void *v)
{
    return DMEMDUP (v, strlen(v)+1, "string");
};

int my_strcmp(void *v1, void *v2)
{
    //printf ("%s(%s, %s)\n", __func__, (char*)v1, (char*)v2);
    return strcmp ((char*)v1, (char*)v2);
};

int main() 
{
    rbtree *t = rbtree_create(true, "test", compare_tetra);
    rbtree *new_t = rbtree_create(true, "new_test", compare_tetra);
    rbtree *t2, *t3;
    size_t key_prev, key_next;
    char *value_prev, *value_next;
    
    printf ("enumerate (should be empty):\n");
    rbtree_foreach(t, visitor, NULL, NULL);

    printf ("rbtree_empty (should be empty): %d\n", rbtree_empty(t));

    rbtree_insert(t, (void*)12345, (void*)"value1");
    printf ("rbtree_empty (should be present something): %d\n", rbtree_empty(t));
    oassert(strcmp (rbtree_lookup(t, (void*)12345), "value1")==0);

    rbtree_insert(t, (void*)12346, (void*)"value2");
    printf ("rbtree_empty (should be present something): %d\n", rbtree_empty(t));

    printf ("enumerate:\n");
    rbtree_foreach(t, visitor, NULL, NULL);

    rbtree_delete(t, (void*)12345);

    rbtree_insert(t, (void*)12347, (void*)"value3");
    rbtree_insert(t, (void*)666, (void*)"666");
    rbtree_insert(t, (void*)0, (void*)"zero!");

    printf ("enumerate:\n");
    rbtree_foreach(t, visitor, NULL, NULL);
    printf ("minimum: %d\n", (int)rbtree_minimum(t)->key);
    printf ("maximum: %d\n", (int)rbtree_maximum(t)->key);

    tst_succ_pred(t);

    rbtree_copy (t, new_t, key_copier, value_copier);
    printf ("enumerate new_t:\n");
    rbtree_foreach(t, visitor, NULL, NULL);

    printf ("freeing:\n");
    rbtree_foreach(new_t, NULL, NULL, dfree);
    rbtree_clear(new_t);
    printf ("rbtree_empty (should be empty after _clear): %d\n", rbtree_empty(new_t));

    rbtree_deinit(t);
    rbtree_deinit(new_t);

    // test 2
    printf ("test 2 (lower/upper bounds)\n");

    t2=rbtree_create(true, "test", compare_tetra);

    //rbtree_insert (t2, (void*)0, "");
    rbtree_insert (t2, (void*)50, "value 50");
    rbtree_insert (t2, (void*)99, "value 99");
    rbtree_insert (t2, (void*)101, "value 101");
    rbtree_insert (t2, (void*)500, "value 500");
    rbtree_insert (t2, (void*)12200, "value 12200");
    rbtree_insert (t2, (void*)12301, "value 12301");
    rbtree_insert (t2, (void*)50000, "value 50000");
    rbtree_insert (t2, (void*)60000, "value 60000");
    printf ("depth=%d\n", rbtree_depth(t2));
    
    printf ("enumerate t2:\n");
    rbtree_foreach(t2, visitor, NULL, NULL);
    printf ("count: %d\n", rbtree_count (t2));
    rbtree_lookup2(t2, (void*)10, (void**)&key_prev, (void**)&value_prev, (void**)&key_next, (void**)&value_next);
    printf ("while looking for 10, key_prev=" PRI_SIZE_T ", value_prev=%s, key_next=" PRI_SIZE_T ", value_next=%s\n", key_prev, value_prev, key_next, value_next);

    rbtree_lookup2(t2, (void*)100, (void**)&key_prev, (void**)&value_prev, (void**)&key_next, (void**)&value_next);
    printf ("while looking for 100, key_prev=" PRI_SIZE_T ", value_prev=%s, key_next=" PRI_SIZE_T ", value_next=%s\n", key_prev, value_prev, key_next, value_next);
    rbtree_lookup2(t2, (void*)101, (void**)&key_prev, (void**)&value_prev, (void**)&key_next, (void**)&value_next);
    printf ("while looking for 101, key_prev=" PRI_SIZE_T ", value_prev=%s, key_next=" PRI_SIZE_T ", value_next=%s\n", key_prev, value_prev, key_next, value_next);
    rbtree_lookup2(t2, (void*)12300, (void**)&key_prev, (void**)&value_prev, (void**)&key_next, (void**)&value_next);
    printf ("while looking for 12300, key_prev=" PRI_SIZE_T ", value_prev=%s, key_next=" PRI_SIZE_T ", value_next=%s\n", key_prev, value_prev, key_next, value_next);

    rbtree_lookup2(t2, (void*)99999, (void**)&key_prev, (void**)&value_prev, (void**)&key_next, (void**)&value_next);
    printf ("while looking for 99999, key_prev=" PRI_SIZE_T ", value_prev=%s, key_next=" PRI_SIZE_T ", value_next=%s\n", key_prev, value_prev, key_next, value_next);
    test_return_all_keys(t2);

    rbtree_deinit(t2);

    printf ("test 3\n");

    t3=rbtree_create(true, "test", compare_tetra);
    rbtree_insert (t3, (void*)50, "value 50");
    rbtree_insert (t3, (void*)99, "value 99");
    printf ("rbtree_empty (should be present something): %d\n", rbtree_empty(t3));
    rbtree_delete(t3, (void*)50);
    rbtree_delete(t3, (void*)99);
    printf ("rbtree_empty (should be empty): %d\n", rbtree_empty(t3));
    rbtree_deinit(t3);

    printf ("test 4\n");
    rbtree *t4=rbtree_create(true, "test 4", my_strcmp);
    char *s1="value";
    char *s2=DSTRDUP(s1, "char*");
   
    //printf ("s1=0x%p\n", s1);
    //printf ("s2=0x%p\n", s2);
    rbtree_insert (t4, (void*)s1, NULL);
   
    if (rbtree_is_key_present (t4, (void*)s2))
        printf ("found\n");
    else
        printf ("not found\n");
    DFREE(s2);
    rbtree_deinit(t4);

    dump_unfreed_blocks();

    return 0;
}

/* vim: set expandtab ts=4 sw=4 : */
