#include <stdlib.h>
#include <stddef.h>

#include "assert.h"
#include "except.h"
#include "mem.h"

////////////////////////////////////////////////////////////////////////////////
// Data
////////////////////////////////////////////////////////////////////////////////

const Except_t mem_failed = { "Allocation Failed" };

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
void* mem_alloc(long nbytes, const char* file, int line)
{
  void* ptr;

  assert(nbytes > 0);
  ptr = malloc(nbytes);
  if (ptr == NULL) {
    /* raise mem_failed exception */
    if (file == NULL) {
      RAISE(mem_failed);
    } else {
      except_raise(&mem_failed, file, line);
    }
    /* raise mem_failed exception */
  }

  return ptr;
} 

////////////////////////////////////////////////////////////////////////////////
///
void* mem_calloc(long count, long nbytes, const char* file, int line)
{
  void* ptr;
  assert(count > 0);
  assert(nbytes > 0);

  ptr = calloc(count, nbytes);
  if (ptr == NULL) {
    if (file == NULL) {
      RAISE(mem_failed);
    } else {
      except_raise(&mem_failed, file, line);
    }
  }

  return ptr;
}


////////////////////////////////////////////////////////////////////////////////
///
void* mem_resize(void* ptr, long nbytes, const char* file, int line)
{
  assert(ptr);
  assert(nbytes > 0);
  ptr = realloc(ptr, nbytes);
  if (ptr == NULL) {
    if (file == NULL) {
      RAISE(mem_failed);
    } else {
      except_raise(&mem_failed, file, line);
    }
  }

  return ptr;
}
