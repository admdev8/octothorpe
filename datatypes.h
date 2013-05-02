#pragma once

#include <stdint.h>

// D.Knuth's MMIX datatypes

typedef uint8_t byte;
typedef uint16_t wyde;
typedef uint32_t tetrabyte;
typedef uint64_t octabyte;

#ifdef __GNUC__
#define OCTABYTE_1 1LL
#endif
#ifdef _MSC_VER
#define OCTABYTE_1 1i64
#endif
