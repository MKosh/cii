#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#include "atom.h"
#include "table.h"
#include "mem.h"
#include "getword.h"

////////////////////////////////////////////////////////////////////////////////
/// Prototypes
void wf(char* name, FILE* fp);
int first(int c);
int rest (int c);
int compare(const void* x, const void* y);
/// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///
int compare(const void* x, const void* y) {
  return strcmp(*(char**)x, *(char**)y);
}

////////////////////////////////////////////////////////////////////////////////
///
void wf(char* name, FILE* fp) {
  Table_T table = Table_new(0, NULL, NULL);
  char buf[128];

  while (getword(fp, buf, sizeof(buf), first, rest)) {
    const char* word;
    int i;
    int* count;

    for (i = 0; buf[i] != '\0'; i++) {
      buf[i] = tolower(buf[i]);
    }

    word = atom_string(buf);
    count = Table_get(table, word);
    if (count) {
      (*count)++;
    } else {
      NEW(count);
      *count = 1;
      Table_put(table, word, count);
    }
  }

  if (name) {
    printf("%s:\n", name);
  }

  /* Print the words */
  {
    int i;
    void** array = Table_toArray(table, NULL);
    qsort(array, Table_length(table), 2 * sizeof(*array), compare);
    for (i = 0; array[i]; i += 2) {
      printf("%d\t%s\n", *(int*)array[i+1], (char*)array[i]);
    }
    FREE(array);
  }

  /* Deallocate the entries and table */
  Table_dealloc(&table);
}

////////////////////////////////////////////////////////////////////////////////
///
int main(int argc, char* argv[]) {
  int i;

  for (i = 1; i < argc; i++) {
    FILE* fp = fopen(argv[i], "r");
    if (fp == NULL) {
      fprintf(stderr, "%s: can't open '%s' (%s)\n", argv[0], argv[i], strerror(errno));
      return EXIT_FAILURE;
    } else {
      wf(argv[i], fp);
      fclose(fp);
    }
  }

  if (argc == 1) wf(NULL, stdin);
  return EXIT_SUCCESS;
}

