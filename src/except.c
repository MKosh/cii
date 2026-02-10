#include "except.h"

#include <stdlib.h>
#include <stdio.h>
#include "assert.h"

#define T Except_t

ExceptFrame* except_stack = NULL;

void except_raise(const T* e, const char* file, int line)
{
  ExceptFrame* p = except_stack;

  assert(e);
  if (p == NULL) {
    /* announce an uncaught exception */
  }

  p->exception = e;
  p->file = file;
  p->line = line;
  /* pop */
  except_stack = except_stack->prev;
  longjmp(p->env, EXCEPT_RAISED);
}

int add(int a, int b)
{
  return a + b;
}

