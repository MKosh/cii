#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "except.h"
#include "mem.h"
#include "arena.h"

void throw(void)
{

}

int main(int argc, char *argv[])
{
  printf("Hello world\n");
  Arena_t allocator = arena_new();
  // int* x = arena_alloc(allocator, sizeof(int), __FILE__, __LINE__);
  int* x = AALLOC(allocator, sizeof(int));

  *x = 5;
  printf("x = %d\n", *x);

  arena_dispose(&allocator);
  return EXIT_SUCCESS;
}
