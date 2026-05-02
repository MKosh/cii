#pragma once

#include "types.h"

typedef struct Array Array;

extern Array* array_new   (i32 length, i32 size);
extern void   array_free  (Array** array);
extern int    array_length(Array* array);
extern int    array_size  (Array* array);
extern void*  array_get   (Array* array, i32 i);
extern void*  array_put   (Array* array, i32 i, void* elem);
extern void   array_resize(Array* array, i32 length);
extern Array* array_copy  (Array* array, i32 length);
