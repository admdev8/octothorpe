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

