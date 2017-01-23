#pragma once

#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

int Base64decode(char *bufplain, const char *bufcoded);
bool likely_base64_string (char* s);

// without padding symbol!
bool is_base64_char(char c);

#ifdef  __cplusplus
}
#endif

