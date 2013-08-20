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

#include "dlist.h"
#include "dmalloc.h"

int main()
{
    dlist *l=dlist_init();

    dlist_insert_at_begin (l, DSTRDUP("1st", "string"));
    dlist_insert_at_begin (l, DSTRDUP("2nd", "string"));
    dlist_insert_at_begin (l, DSTRDUP("3rd", "string"));

    printf ("dlist_get_first=%s\n", (char*)dlist_get_first(l));

    printf ("enumerate:\n");

    for (dlist *i=l; i; i=i->next)
        printf ("%s\n", (char*)i->data);

    dlist_free(l, dfree);
    
    dump_unfreed_blocks();

    return 0;
};

/* vim: set expandtab ts=4 sw=4 : */
