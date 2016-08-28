#pragma once

#include <stdbool.h>

int Base64decode(char *bufplain, const char *bufcoded);
bool likely_base64_string (char* s);

// without padding symbol!
bool is_base64_char(char c);
