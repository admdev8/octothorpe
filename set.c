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

#include "set.h"
#include "dmalloc.h"

void set_of_REG_to_string (rbtree *t, strbuf *out, unsigned limit)
{
    unsigned cnt=rbtree_count(t);
    REG *keys=DMALLOC(REG, cnt, "keys");
    rbtree_return_all_keys (t, (void**)keys);
    make_compact_list_of_REGs (keys, cnt, out, limit);
    DFREE (keys);
};

// FIXME: how to join these two functions into one?
void set_of_doubles_to_string (rbtree *t, strbuf *out, unsigned limit)
{
    unsigned doubles_cnt=rbtree_count(t);
    unsigned doubles_dumped=0;
    rbtree_node *max=rbtree_maximum(t);
    for (rbtree_node *j=rbtree_minimum(t); j; j=rbtree_succ(j))
    {
        strbuf_addf (out, "%.1f", *(double*)j->key);
        if (doubles_dumped++>limit)
        {
            strbuf_addf (out, " (%d doubles skipped)", doubles_cnt - doubles_dumped);
            break;
        };
        if (j!=max)
            strbuf_addstr (out, ", ");
    };
};

void set_of_string_to_string (rbtree *t, strbuf *out, unsigned limit)
{
    unsigned strings_cnt=rbtree_count(t);
    unsigned strings_dumped=0;
    rbtree_node *max=rbtree_maximum(t);
    for (rbtree_node *j=rbtree_minimum(t); j; j=rbtree_succ(j))
    {
        strbuf_addstr (out, (char*)j->key);
        if (strings_dumped++>limit)
        {
            strbuf_addf (out, " (%d strings skipped)", strings_cnt - strings_dumped);
            break;
        };
        if (j!=max)
            strbuf_addstr (out, ", ");
    };
};

void set_add_string_or_free (rbtree *t, const char *s)
{
    if (rbtree_is_key_present(t, (void*)s))
        DFREE(s);
    else
        rbtree_insert(t, (void*)s, NULL);
};

/* vim: set expandtab ts=4 sw=4 : */
