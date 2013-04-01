#pragma once

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

unsigned most_significant_hex_number(uint64_t x);
void die (const char * fmt, ...);
void* memdup (void *p, size_t s);

#ifdef  __cplusplus
}
#endif
