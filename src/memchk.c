#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "except.h"
#include "mem.h"

////////////////////////////////////////////////////////////////////////////////
/// checking types
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///
union align {
  int i;
  long l;
  long* lp;
  void* p;
  void (*fp)(void);
  float f;
  double d;
  long double ld;
};

////////////////////////////////////////////////////////////////////////////////
/// checking macros
////////////////////////////////////////////////////////////////////////////////

#define hash(p, t) (((unsigned long)(p) >> 3) & (sizeof(t)/sizeof((t)[0]) - 1))

#define NDESCRIPTORS 512

#define NALLOC ((4096 + sizeof(union align) - 1) / (sizeof(union align))) * sizeof(union align)

////////////////////////////////////////////////////////////////////////////////
/// data
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// checking data
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
static struct descriptor {
  struct descriptor* free;
  struct descriptor* link;
  const void* ptr;
  long size;
  const char* file;
  int line;
}* htab[2048];

static struct descriptor freelist = { &freelist };


////////////////////////////////////////////////////////////////////////////////
/// checking functions
////////////////////////////////////////////////////////////////////////////////

static struct descriptor* find(const void* ptr)
{
  struct descriptor* bp = htab[hash(ptr, htab)];

  while (bp && bp->ptr != ptr) {
    bp = bp->link;
  }

  return bp;
}

void mem_free(void* ptr, const char* file, int line)
{
  if (ptr) {
    struct descriptor* bp;
    /* v set bp if ptr is valid v */
    if (((unsigned long)ptr) % (sizeof(union align)) != 0
        || (bp = find(ptr)) == NULL
        || bp->free ) {
      except_raise(&assert_failed, file, line);
    }
    /* ^ set bp if ptr is valid ^ */
    bp->free = freelist.free;
    freelist.free = bp;
  }
}

void* mem_resize(void* ptr, long nbytes, const char* file, int line)
{
  struct descriptor* bp;
  void* newptr;

  assert(ptr);
  assert(nbytes > 0);
  /* v set bp if ptr is valid v */
  if (((unsigned long)ptr) % (sizeof(union align)) != 0
      || (bp = find(ptr)) == NULL
      || bp->free ) {
    except_raise(&assert_failed, file, line);
  }
  /* ^ set bp if ptr is valid ^ */
  newptr = mem_alloc(nbytes, file, line);
  memcpy(newptr, ptr, nbytes < bp->size ? nbytes : bp->size);
  mem_free(ptr, file, line);

  return newptr;
}

void* mem_calloc(long count, long nbytes, const char* file, int line)
{
  void* ptr;

  assert(count > 0);
  assert(nbytes > 0);

  ptr = mem_alloc(count*nbytes, file, line);
  memset(ptr, '\0', count*nbytes);

  return ptr;
}

static struct descriptor* dalloc(void* ptr, long size, const char* file, int line)
{
  static struct descriptor* avail;
  static int nleft;

  if (nleft <= 0) {
    /* allocate descriptors */
    avail = malloc(NDESCRIPTORS * sizeof(*avail));
    if (avail == NULL) {
      return NULL;
    }
    /* allocate descriptors */
    nleft = NDESCRIPTORS;
  }

  avail->ptr = ptr;
  avail->size = size;
  avail->file = file;
  avail->line = line;
  avail->free = avail->link = NULL;
  nleft--;

  return avail++;
}

void* mem_alloc(long nbytes, const char* file, int line)
{
  struct descriptor* bp;
  void* ptr;

  assert(nbytes > 0);
  /* round nbytes up to an alignment boundary */
  nbytes = ((nbytes + sizeof(union align) - 1) / (sizeof(union align))) * sizeof(union align);
  /* round nbytes up to an alignment boundary */
  for (bp = freelist.free; bp; bp = bp->free) {
    if (bp->size > nbytes) {
      /* use the end of the block at bp->ptr */
      bp->size -= nbytes;
      ptr = (char*)bp->ptr + bp->size;
      if ((bp = dalloc(ptr, nbytes, file, line)) != NULL) {
        unsigned h = hash(ptr, htab);
        bp->link = htab[h];
        htab[h] = bp;
        return ptr;
      } else {
        /* raise mem_failed */
        if (file == NULL) {
          RAISE(mem_failed);
        } else {
          except_raise(&mem_failed, file, line);
        }
        /* raise mem_failed */
      }
      /* use the end of the block at bp->ptr */
    }
    if (bp == &freelist) {
      struct descriptor* newptr;
      /* newptr <- a block of size NALLOC + nbytes */
      if ((ptr = malloc(nbytes + NALLOC)) == NULL
          || (newptr = dalloc(ptr, nbytes + NALLOC, __FILE__, __LINE__)) == NULL) {
        if (file == NULL) {
          RAISE(mem_failed);
        } else {
          except_raise(&mem_failed, file, line);
        }
      }
      /* newptr <- a block of size NALLOC + nbytes */
      newptr->free = freelist.free;
      freelist.free = newptr;
    }
  }

  assert(0);
  return NULL;
}

