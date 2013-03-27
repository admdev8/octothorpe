#include "memory.h"

#ifndef bzero
void bzero (void* ptr, size_t s)
{
    memset(ptr, 0, s);
};
#endif

