#include "array.h"
#include "types.h"
#include "assert.h"
#include "mem.h"
#include "arrayrep.h"

// #include <bits/pthreadtypes-arch.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
///
Array* array_new(i32 length, i32 size)
{
  Array* array;
  NEW(array);
  if (length > 0) {
    arrayrep_init(array, length, size, CALLOC(length, size));
  } else {
    arrayrep_init(array, length, size, NULL);
  }
  return array;
}

////////////////////////////////////////////////////////////////////////////////
///
void array_free(Array** array)
{
  assert(array && *array);
  FREE((*array)->array);
  FREE(*array);
}

////////////////////////////////////////////////////////////////////////////////
///
int array_length(Array* array)
{
  assert(array);
  return array->length;
}

////////////////////////////////////////////////////////////////////////////////
///
i32 array_size (Array* array)
{
  assert(array);
  return array->size;
}

////////////////////////////////////////////////////////////////////////////////
///
void* array_get(Array* array, int i)
{
  assert(array);
  assert(i >= 0 && i < array->length);
  return array->array + i * array->size;
}

////////////////////////////////////////////////////////////////////////////////
///
void* array_put(Array* array, int i, void* elem)
{
  assert(array);
  assert(i >= 0 && i < array->length);
  assert(elem);
  memcpy(array->array + i * array->size, elem, array->size);
  return elem;
}

////////////////////////////////////////////////////////////////////////////////
///
void array_resize(Array* array, int length)
{
  assert(array);
  assert(length >= 0);
  if (length == 0) {
    FREE(array->array);
  } else if (array->length == 0) {
    array->array = ALLOC(length * array->size);
  } else {
    RESIZE(array->array, length * array->size);
  }

  array->length = length;
}

////////////////////////////////////////////////////////////////////////////////
///
Array* array_copy(Array* array, int length)
{
  Array* copy;
  assert(array);
  assert(length >= 0);

  copy = array_new(length, array->size);
  if (copy->length >= array->length && array->length > 0) {
    memcpy(copy->array, array->array, array->length*array->size);
  } else if (array->length > copy->length && copy->length > 0) {
    memcpy(copy->array, array->array, copy->length * array->size);
  }

  return copy;
}

////////////////////////////////////////////////////////////////////////////////
///
void arrayrep_init(Array* array, i32 length, i32 size, void* ary)
{
  assert(array);
  assert(ary && length > 0 || length == 0 && ary == NULL);
  assert(size > 0);
  array->length = length;
  array->size = size;

  if (length > 0) {
    array->array = ary;
  } else {
    array->array = NULL;
  }
}
