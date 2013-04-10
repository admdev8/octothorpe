#pragma once

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>

// Idea: нужно ли кому-то знать, сколько выделено байт по такому-то адресу? 
// тут такое можно реализовать

#ifdef _DEBUG
void* dmalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname);
#define DMALLOC(type, size, comment) ((type*)dmalloc(sizeof(type)*size, __FILE__, __LINE__, __FUNCTION__, comment))
#else
#define DMALLOC(type, size, comment) ((type*)malloc(sizeof(type)*size))
#endif

#ifdef _DEBUG
void* drealloc (void* ptr, size_t size, const char * filename, unsigned line, const char * function, const char * structname);
#define DREALLOC(ptr, type, size, comment) ((type*)drealloc(ptr, sizeof(type)*size, __FILE__, __LINE__, __FUNCTION__, comment))
#else
#define DREALLOC(ptr, type, size, comment) ((type*)drealloc(ptr, sizeof(type)*size))
#endif

#ifdef _DEBUG
void* dcalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname);
#define DCALLOC(type, size, comment) ((type*)dcalloc(sizeof(type)*size, __FILE__, __LINE__, __FUNCTION__, comment))
#else
#define DCALLOC(type, size, comment) ((type*)calloc(size, sizeof(type)))
#endif

void dfree (void* ptr);

// for symmetry!
#ifdef _DEBUG
#define DFREE(p) (dfree(p))
#else
#define DFREE(p) (free(p))
#endif

#ifdef _DEBUG
char* dstrdup (const char *s, const char * filename, unsigned line, const char * function, const char * structname);
#define DSTRDUP(size,comment) (dstrdup(size, __FILE__, __LINE__, __FUNCTION__, comment))
#else
#define DSTRDUP(size,comment) (strdup(size))
#endif

void* dmemdup (void *p, size_t s, const char * filename, unsigned line, const char * function, const char * structname);

#define DMEMDUP(ptr,size,comment) (dmemdup(ptr, size, __FILE__, __LINE__, __FUNCTION__, comment))

void dump_unfreed_blocks();
void dmalloc_deinit();

void dmalloc_break_at_seq_n (unsigned seq_n);

#ifdef  __cplusplus
}
#endif
