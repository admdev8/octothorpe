#pragma once

#include <stdint.h>

// D.Knuth's MMIX datatypes

typedef uint8_t byte;
typedef uint16_t wyde;
typedef uint32_t tetrabyte;
typedef int32_t tetrabyte_s;
typedef uint64_t octabyte;
typedef int64_t octabyte_s;

#ifdef __GNUC__
#define OCTABYTE_1 1LL
#endif
#ifdef _MSC_VER
#define OCTABYTE_1 1i64
#endif

#ifdef _WIN64
#define REG_1 OCTABYTE_1
typedef octabyte REG;
typedef octabyte_s SIGNED_REG;
#define REG_SIZE 8
#else
#define REG_1 1
typedef tetrabyte REG;
typedef tetrabyte_s SIGNED_REG;
#define REG_SIZE 4
#endif
