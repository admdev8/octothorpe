#include "dmalloc.h"
#include <stdint.h>

void main()
{
    uint8_t* t1=DMALLOC(uint8_t, 6, "block123");
    uint8_t* t2=DMALLOC(uint8_t, 124, "block124");
    uint8_t* t3=DMALLOC(uint8_t, 12,  "block12");
    uint8_t* t4=DMALLOC(uint8_t, 555, "block555");
    uint8_t* t5=DMALLOC(uint8_t, 666, "block666");
    uint8_t* t6=DMALLOC(uint8_t, 777, "block777");

    char *s=DSTRDUP ("hahaha", "s");

    t6=DREALLOC(t6, uint8_t, 888, "new size for block777");

    // guards are checking?
    //memcpy (t1, "12345678", 8);
    //memcpy((uint8_t*)t1-1, "a", 1);

    DFREE(t1);
    DFREE(t5);
    DFREE(t6);
    DFREE(s);

    dump_unfreed_blocks();
    dmalloc_deinit();
};
