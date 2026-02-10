#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "except.h"

void throw(void)
{

}

int main(int argc, char *argv[])
{
  printf("Hello world\n");
  TRY
    throw();
  ELSE
    fprintf(stderr, "An internal error occurred\n");
    RERAISE;
  END_TRY;
  return EXIT_SUCCESS;
}
