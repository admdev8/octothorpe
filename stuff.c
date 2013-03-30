#include <stdint.h>

unsigned most_significant_hex_number(uint64_t x)
{
    uint64_t t=x;
    int i;

    // _BitScanReverse64 can be used here, probably (?)

    for (i=0; i<(64/4); i++)
    {
        if (t&0xF000000000000000)
        {
            //printf (__FUNCTION__"(0x%016llX) -> 0x%X\n", x, t>>(64-4));
            return t>>(64-4);
        }
        else
            t=t<<4;
    };
    //printf (__FUNCTION__"(0x%016llX) -> 0\n", x);
    return 0;
};
