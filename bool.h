#pragma once

#ifndef _WIN32

#ifndef BOOL
#define BOOL unsigned
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#else

// import win32 BOOL
#include <windows.h>

#endif
