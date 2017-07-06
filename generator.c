#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *get_dependencies(FILE *file);
int get_o_files(char *argv[], int argc, char *argo[]);

int main(int argc, char *argv[]) {

  // Constant array containing all standard C libraries
  const char *standard_headers[] = {"stdio.h", "string.h", "assert.h", "complex.h",
  "ctype.h", "errno.h", "fenv.h", "float.h", "inttypes.h", "iso646.h", "limits.h",
  "locale.h", "setjmp.h", "signal.h", "stdalign.h", "stdarg.h", "stdatomic.h", "stdbool",
  "stddef.h", "stdint.h", "stdlib.h", "stdnoreturn.h", "tgmath.h", "threads.h",
  "uchar.h", "wchar.h", "wctype.h"};

  char name[50]; // string for final .exe name
  char *argo[argc -1]; // array of strings to hold .o versions of given .c files
  char cflags[120]; // string to hold desired CFlags
  FILE *make = fopen("Makefile", "w"); // File pointer to create/print to Makefile

  printf("Give name of final executable: "); // asking user for name of the final .exe
  scanf("%s", name); // scanning the value into the name var

  printf("Give Cflags: "); // asking user for desired CFlags
  fgets(cflags, 120, stdin); // first fgets to get to next line
  fgets(cflags, 120, stdin); // second fgets to read user input
  cflags[strlen(cflags) - 1] = '\0'; // getting rid of \n at the end of CFlags

  fprintf(make, "all: %s\n\n%s:", name, name); // printing the all rule

  get_o_files(argv, argc, argo); // getting array of .o files from the .c files

  // printing dependencies for the .exe rule
  for(int i = 0; i < argc - 1; i++) {
    fprintf(make, " %s", argo[i]);
  }

  //printing second line of the .exe rule
  fprintf(make, "\n\tgcc %s -o %s", cflags, name);
  for(int i = 0; i < argc - 1; i++) {
    fprintf(make, " %s", argo[i]);
  }

  fprintf(make, "\n\n"); // going to next rule

  return 0;
}

char *get_dependencies(FILE *file) {
  return 0;
}

int get_o_files(char *argv[], int argc, char *argo[]) {

  char current[50];

  for(int i = 0; i < argc - 1; i++) {
    strcpy(current, argv[i + 1]);
    current[strlen(current) - 1] = 'o';
    argo[i] = (char*)malloc(strlen(current));
    strcpy(argo[i], current);
    /*for(int j = 0; current[j] != '/0'; j++) {
      current[j - 1] = 'o';
    } */
  }

  return 0;
}
