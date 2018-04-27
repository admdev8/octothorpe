#pragma once

#include "datatypes.h"
#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

double entropy (byte* buf, size_t bufsize);

// in fixed point format
int32_t entropy_int (byte* buf, size_t bufsize);

#ifdef  __cplusplus
}
#endif

