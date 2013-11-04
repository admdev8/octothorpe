#pragma once

#ifdef _WIN64

#define O_BITS64

#elif _WIN32

#define O_BITS32

#elif __APPLE__

	#ifdef __ppc__
	#define O_BITS32
	#elif __ppc64__
	#define O_BITS64
	#elif __i386__
	#define O_BITS32
	#elif __x86_64__
	#define O_BITS64
	#else
	#error "(Apple) taget CPU was not detected"
	#endif
	
#elif __linux__

	#if defined(__LP64__) || defined (_LP64)
	#define O_BITS64
	#else
	#define O_BITS32
	#endif

#else
#error "OS was not detected"
#endif

