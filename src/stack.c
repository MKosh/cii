#include <stddef.h>

#include "assert.h"
#include "mem.h"
#include "stack.h"
#include "types.h"

#define T Stack_t

////////////////////////////////////////////////////////////////////////////////
///
struct T {
  int count;
  struct elem {
    void* x;
    struct elem* link;
  }* head;
};

////////////////////////////////////////////////////////////////////////////////
///
T Stack_new(void)
{
  T stack;

  NEW(stack);
  stack->count = 0;
  stack->head = NULL;

  return stack;
}

////////////////////////////////////////////////////////////////////////////////
///
int stack_empty(T stack)
{
  assert(stack);
  return stack->count == 0;
}

////////////////////////////////////////////////////////////////////////////////
///
void stack_push(T stk, void* x)
{
  struct elem* t;

  assert(stk);
  NEW(t);
  t->x = x;
  t->link = stk->head;
  stk->head = t;
  stk->count++;
}

////////////////////////////////////////////////////////////////////////////////
///
void* stack_pop(T stack)
{
  void* x;
  struct elem* t;

  assert(stack);
  assert(stack->count > 0);
  t = stack->head;

  stack->head = t->link;
  stack->count--;
  x = t->x;
  FREE(t);
  return x;
}

////////////////////////////////////////////////////////////////////////////////
///
void stack_free(T* stack)
{
  struct elem *t, *u;
  assert(stack && *stack);
  for (t = (*stack)->head; t; t = u) {
    u = t->link;
    FREE(t);
  }

  FREE(*stack);
}
