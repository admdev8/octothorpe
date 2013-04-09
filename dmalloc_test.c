#include "dmalloc.h"

void main()
{
    void* t1=DMALLOC(123, "block123");
    void* t2=DMALLOC(124, "block124");
    void* t3=DMALLOC(12,  "block12");
    void* t4=DMALLOC(555, "block555");
    void* t5=DMALLOC(666, "block666");
    void* t6=DMALLOC(777, "block777");

    char *s=DSTRDUP ("hahaha", "s");

    t6=DREALLOC(t6, 888, "new size for block777");

    DFREE(t1);
    DFREE(t5);
    DFREE(t6);
    DFREE(s);

    dump_unfreed_blocks();
    dmalloc_deinit();
};
