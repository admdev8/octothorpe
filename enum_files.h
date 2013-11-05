#pragma once

#include <stdbool.h>

typedef void (*callback_fn)(const char *name, const char *pathname, time_t t, bool is_dir, void *param);
void enum_files_in_dir(const char* path, callback_fn cb, void *param);
