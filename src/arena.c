#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "except.h"
#include "arena.h"

#define T Arena_t

const Except_t Arena_NewFailed = { "Arena Creation Failed" };
const Except_t Arena_Failed = { "Arena Allocation Failed" };

////////////////////////////////////////////////////////////////////////////////
/// macros
////////////////////////////////////////////////////////////////////////////////

#define THRESHOLD 10

////////////////////////////////////////////////////////////////////////////////
/// types
////////////////////////////////////////////////////////////////////////////////

struct T {
  T prev;
  char* avail;
  char* limit;
};

union align {
  int i;
  long l;
  long *lp;
  void* p;
  void (*fp)(void);
  float f;
  double d;
  long double ld;
};

union header {
  struct T b;
  union align a;
};

////////////////////////////////////////////////////////////////////////////////
/// data
////////////////////////////////////////////////////////////////////////////////

static T freechunks;
static int nfree;

////////////////////////////////////////////////////////////////////////////////
/// funcions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
T arena_new(void)
{
  T arena = malloc(sizeof(*arena));
  if (arena == NULL) {
    RAISE(Arena_NewFailed);
  }

  arena->prev = NULL;
  arena->limit = arena->avail = NULL;
  return arena;
}

////////////////////////////////////////////////////////////////////////////////
///
void arena_dispose(T* ap)
{
  assert(ap && *ap);
  arena_free(*ap);
  free(*ap);
  *ap = NULL;
}

////////////////////////////////////////////////////////////////////////////////
///
void* arena_alloc(T arena, long nbytes, const char* file, int line)
{
  assert(arena);
  assert(nbytes > 0);
  // round nbytes up to an alignment boundary
  nbytes = ((nbytes + sizeof(union align) - 1) / (sizeof(union align))) * sizeof(union align);
  while (nbytes > arena->limit - arena->avail) {
    /* get a new chunk */
    T ptr;
    char* limit;
    /* ptr <- a new chunk */
    if ((ptr = freechunks) != NULL) {
      freechunks = freechunks->prev;
      nfree--;
      limit = ptr->limit;
    } else {
      long m = sizeof(union header) + nbytes + 1024 * 1024;
      ptr = malloc(m);
      if (ptr == NULL) {
        if (file == NULL) {
          RAISE(Arena_Failed);
        } else {
          except_raise(&Arena_Failed, file, line);
        }
      }
      limit = (char*)ptr + m;
    }
    *ptr = *arena;
    arena->avail = (char*)((union header*)ptr + 1);
    arena->limit = limit;
    arena->prev = ptr;
  }
  arena->avail += nbytes;
  return arena->avail - nbytes;
}

////////////////////////////////////////////////////////////////////////////////
///
void* arena_calloc(T arena, long count, long nbytes, const char* file, int line)
{
  void* ptr;

  assert(count > 0);
  ptr = arena_alloc(arena, count*nbytes, file, line);
  memset(ptr, '\0', count*nbytes);
  return ptr;
}

////////////////////////////////////////////////////////////////////////////////
///
void arena_free(T arena)
{
  assert(arena);
  while (arena->prev) {
    struct T tmp = *arena->prev;
    /* free the chunk described by arena */
    if (nfree < THRESHOLD) {
      arena->prev->prev = freechunks;
      freechunks = arena->prev;
      nfree++;
      freechunks->limit = arena->limit;
    } else {
      free(arena->prev);
    }
    *arena = tmp;
  }
}
