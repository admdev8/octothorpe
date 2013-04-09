#pragma once

#ifndef _WIN32

// TODO: потом, когда я переколбашу tracer полностью на Си, надо заюзать stdbool вместо этого!

#ifndef BOOL
#define BOOL unsigned
#endif

#ifndef FALSE
// Rationale: BOOL variables should be cleared by calloc() call
#define FALSE 0
#endif

#ifndef TRUE
// Rationale: this should be workin in if(...), while(...), etc, statements
#define TRUE 1
#endif

#else

// import win32 BOOL
#include <windows.h>

#endif
