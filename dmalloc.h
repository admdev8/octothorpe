#pragma once

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>

void* dmalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname);

#ifdef _DEBUG
#define DMALLOC(s, st) (dmalloc(s, __FILE__, __LINE__, __FUNCTION__, st))
#else
#define DMALLOC(s, st) (malloc(s))
#endif

void* dcalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname);

#ifdef _DEBUG
#define DCALLOC(s, st) (dcalloc(s, __FILE__, __LINE__, __FUNCTION__, st))
#else
#define DCALLOC(s, st) (calloc(s, 1))
#endif

void dfree (void* ptr);

// for symmetry!
#define DFREE(p) (dfree(p))

// TODO: realloc()

char* dstrdup (const char *s, const char * filename, unsigned line, const char * function, const char * structname);

#ifdef _DEBUG
#define DSTRDUP(s,st) (dstrdup(s, __FILE__, __LINE__, __FUNCTION__, st))
#else
#define DSTRDUP(s,st) (dstrdup(s))
#endif

void dump_unfreed_blocks();
void dmalloc_deinit();

#ifdef  __cplusplus
}
#endif
