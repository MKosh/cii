#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "except.h"
#include "mem.h"
#include "arena.h"
#include "list.h"

void throw(void)
{

}

int main(int argc, char *argv[])
{
  printf("Starting.\n");

  List_t p1 = list_list("Atom", "Mem", NULL);

  list_free(&p1);

  printf("Done.\n");

  return EXIT_SUCCESS;
}
