#include "dmalloc.h"
#include "datatypes.h"

void main()
{
    byte* t1=DMALLOC(byte, 6, "block123");
    byte* t2=DMALLOC(byte, 124, "block124");
    byte* t3=DMALLOC(byte, 12,  "block12");
    byte* t4=DMALLOC(byte, 555, "block555");
    byte* t5=DMALLOC(byte, 666, "block666");
    byte* t6=DMALLOC(byte, 777, "block777");

    char *s=DSTRDUP ("hahaha", "s");

    t6=DREALLOC(t6, byte, 888, "new size for block777");

    // guards are checking?
    //memcpy (t1, "12345678", 8);
    //memcpy((byte*)t1-1, "a", 1);

    DFREE(t1);
    DFREE(t5);
    DFREE(t6);
    DFREE(s);

    dump_unfreed_blocks();
    dmalloc_deinit();
};
