#pragma once

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>

#ifdef _DEBUG
void* dmalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname);
#define DMALLOC(s, st) (dmalloc(s, __FILE__, __LINE__, __FUNCTION__, st))
#else
#define DMALLOC(s, st) (malloc(s))
#endif

#ifdef _DEBUG
void* dcalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname);
#define DCALLOC(s, st) (dcalloc(s, __FILE__, __LINE__, __FUNCTION__, st))
#else
#define DCALLOC(s, st) (calloc(s, 1))
#endif

void dfree (void* ptr);

// for symmetry!
#ifdef _DEBUG
#define DFREE(p) (dfree(p))
#else
#define DFREE(p) (free(p))
#endif

// TODO: realloc()

#ifdef _DEBUG
char* dstrdup (const char *s, const char * filename, unsigned line, const char * function, const char * structname);
#define DSTRDUP(s,st) (dstrdup(s, __FILE__, __LINE__, __FUNCTION__, st))
#else
#define DSTRDUP(s,st) (strdup(s))
#endif

void* dmemdup (void *p, size_t s, const char * filename, unsigned line, const char * function, const char * structname);

#define DMEMDUP(p,s,st) (dmemdup(p, s, __FILE__, __LINE__, __FUNCTION__, st))

void dump_unfreed_blocks();
void dmalloc_deinit();

#ifdef  __cplusplus
}
#endif
