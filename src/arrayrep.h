#pragma once

#include "types.h"

struct Array {
  i32 length;
  i32 size;
  u8* array;
};

extern void arrayrep_init(struct Array* array, i32 length, i32 size, void* ary);
