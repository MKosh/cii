#include "assert.h"

const Except_t assert_failed = { "Assertion Failed" };

void (assert)(int e)
{
  assert(e);
}
