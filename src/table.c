#include <limits.h>
#include <stddef.h>
#include "mem.h"
#include "assert.h"
#include "table.h"

struct Table {
  i32 size;
  i32 length;
  u32 timestamp;
  i32 (*cmp)(const void* x, const void* y);
  u64 (*hash)(const void* key);

  struct binding {
    struct binding* link;
    const void* key;
    void* value;
  }** buckets;
};

static i32 cmpatom(const void* x, const void* y)
{
  return x != y;
}

static u64 hashatom(const void* key)
{
  return (u64)key>>2;
}

////////////////////////////////////////////////////////////////////////////////
///
Table* table_new(i32 hint, i32 cmp(const void* x, const void* y), u64 hash(const void* key))
{
  Table* table;
  i32 i;

  static i32 primes[] = { 509, 509, 1021, 2053, 4093, 8191, 16381, 32771, 65521, INT_MAX};

  assert(hint >= 0);

  for (i = 1; primes[i] < hint; i++) {}
  table = ALLOC(sizeof(*table) + primes[i-1] * sizeof(table->buckets[0]));
  table->size = primes[i-1];
  table->cmp = cmp ? cmp : cmpatom;
  table->hash = hash ? hash : hashatom;
  table->buckets = (struct binding**)(table + 1);

  for (i = 0; i < table->size; i++) {
    table->buckets[i] = NULL;
  }

  table->length = 0;
  table->timestamp = 0;

  return table;
}

////////////////////////////////////////////////////////////////////////////////
///
void table_free (Table** table);

////////////////////////////////////////////////////////////////////////////////
///
i32 table_length(Table* table)
{
  assert(table);
  return table->length;
}

////////////////////////////////////////////////////////////////////////////////
///
void* table_put(Table* table, const void* key, void* value)
{
  i32 i;
  struct binding* p;
  void* prev;

  assert(table);
  assert(key);
  /* search table for key */
  i = (*table->hash)(key) % table->size;
  for (p = table->buckets[i]; p; p = p->link) {
    if ((*table->cmp)(key, p->key) == 0) {
      break;
    }
  }

  if (p == NULL) {
    NEW(p);
    p->key = key;
    p->link = table->buckets[i];
    table->buckets[i] = p;
    table->length++;
    prev = NULL;
  } else {
    prev = p->value;
  }

  p->value = value;
  table->timestamp++;
  return prev;
}

////////////////////////////////////////////////////////////////////////////////
///
void*  table_get    (Table* table, const void* key)
{
  i32 i;
  struct binding* p;
  assert(table);
  assert(key);
  /* search table for key */
  i = (*table->hash)(key)%table->size;
  for (p = table->buckets[i]; p; p = p->link) {
    if ((*table->cmp)(key, p->key) == 0) {
      break;
    }
  }
  /* search table for key */
  return p ? p->value : NULL;
}

////////////////////////////////////////////////////////////////////////////////
///
void*  table_remove (Table* table, const void* key);

////////////////////////////////////////////////////////////////////////////////
///
void   table_map    (Table* table, void apply(const void* key, void** value, void* cl), void* cl);

////////////////////////////////////////////////////////////////////////////////
///
void** table_toArray(Table* table, void* end);
