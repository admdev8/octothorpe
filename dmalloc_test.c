#include "dmalloc.h"
#include "datatypes.h"
#include "logging.h"

int main()
{
    byte *t1, *t2, *t3, *t4, *t5, *t6, *t7;
    char *s;
    //L_init("tmp");
    
    s=DSTRDUP ("hahaha", "s");

    t1=DMALLOC(byte, 6, "block123");
    t2=DMALLOC(byte, 124, "block124");
    memcpy (t2, "block124", 8);
    t3=DMALLOC(byte, 12,  "block12");
    memcpy (t3, "block12", 7);
    t4=DMALLOC(byte, 555, "block555");
    memcpy (t4, "block555", 8);
    t5=DMALLOC(byte, 666, "block666");
    t6=DMALLOC(byte, 777, "block777");
    t7=DMALLOC(byte, 0, "empty block");

    t6=DREALLOC(t6, byte, 888, "new size for block777");

    // guards are checking?
    //memcpy (t1, "12345678", 8);
    //memcpy((byte*)t1-1, "a", 1);

    t1=DREALLOC(t1, byte, 100, "new size for block123");

    DFREE(t1);
    DFREE(t5);
    //L_print_buf (t5, 666);
    DFREE(t6);
    DFREE(t7);
    DFREE(s);

    // make compiler happy about unused variables!
    t1=t4; t1=t3; t1=t2;

    dump_unfreed_blocks();
    dmalloc_deinit();

    return 0;
};
