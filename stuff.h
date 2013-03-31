#pragma once

#include <stdint.h>

unsigned most_significant_hex_number(uint64_t x);
void die (const char * fmt, ...);
void* memdup (void *p, size_t s);
