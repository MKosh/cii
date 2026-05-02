#pragma once

#include "types.h"

typedef struct Table Table;
 
extern Table* table_new    (i32 hint, i32 cmp(const void* x, const void* y), u64 hash(const void* key));
extern void   table_free   (Table** table);
extern i32    table_length (Table* table);
extern void*  table_put    (Table* table, const void* key, void* value);
extern void*  table_get    (Table* table, const void* key);
extern void*  table_remove (Table* table, const void* key);
extern void   table_map    (Table* table, void apply(const void* key, void** value, void* cl), void* cl);
extern void** table_toArray(Table* table, void* end);
