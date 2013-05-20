#pragma once

#include "datatypes.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef _WIN64

double cvt80to64 (byte *p);
void cvt64to80 (double in, byte *out);

#else

double __cdecl cvt80to64 (byte *p);
void __cdecl cvt64to80 (double in, byte *out);

#endif

#ifdef  __cplusplus
}
#endif
