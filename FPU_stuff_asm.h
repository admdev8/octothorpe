#pragma once

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef _WIN64

double cvt80to64 (uint8_t *p);
void cvt64to80 (double in, uint8_t *out);

#else

double __cdecl cvt80to64 (uint8_t *p);
void __cdecl cvt64to80 (double in, uint8_t *out);

#endif

#ifdef  __cplusplus
}
#endif
