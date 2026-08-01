#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "except.h"
#include "mem.h"
#include "arena.h"
#include "list.h"
#include "set.h"
#include "atom.h"

void throw(void)
{

}

int main(int argc, char *argv[])
{
  printf("Starting.\n");

  List_t p1 = list_list("Atom", "Mem", NULL);

  list_free(&p1);

  Set_T s1 = Set_new(1, NULL, NULL);
  const char* name = atom_new("Tom", 3);

  Set_put(s1, name);
  if (Set_member(s1, name) == 1) {
    printf("Found member: %s in set\n", name);
  }
  Set_free(&s1);

  printf("Done.\n");

  return EXIT_SUCCESS;
}
