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

#include "string_list.h"
#include "dmalloc.h"

void strlist_insert_at_begin(strlist_node **l, char *s)
{
    strlist_node *n;
    
    if (*l==NULL)
    {
        // allocate initial
        *l=DCALLOC (strlist_node, 1, "strlist_node");
    };

    n=DCALLOC (strlist_node, 1, "strlist_node");
    n->s=(*l)->s;
    n->next=(*l)->next;

    (*l)->next=n;
    (*l)->s=s;
};

char* strlist_get_first(strlist_node *l)
{
    return l->s;
};

void strlist_free (strlist_node* l, void (*free_fn)(void*))
{
    if (l->next)
        strlist_free (l->next, free_fn);

    if (free_fn)
        (*free_fn)(l->s);
    DFREE (l);
};

/* vim: set expandtab ts=4 sw=4 : */
