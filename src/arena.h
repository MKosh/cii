#pragma once

#include "except.h"

#define T Arena_t
typedef struct T* T;

extern const Except_t Arena_NewFailed;
extern const Except_t Areana_Failed;

extern T arena_new(void);
extern void arena_dispose(T* ap);
extern void* arena_alloc(T arena, long nbytes, const char* file, int line);
extern void* arena_calloc(T arena, long count, long nbytes, const char* file, int line);
extern void arena_free(T arena);

#define AALLOC(arena, nbytes) arena_alloc((arena), (nbytes), __FILE__, __LINE__)

#undef T
