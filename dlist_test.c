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
