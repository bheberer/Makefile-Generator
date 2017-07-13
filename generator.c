#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rule_struct.h"

int get_o_files(char *argv[], int argc, char *argo[]);
int init_rule(char *target, Rule *rule);
int print_exe_rule(char *target, char *cflags, char *argo[], FILE *make, int argc);
int get_deps(char *filename, Rule *rule);
int add_dep(char *name, Rule *rule);
int print_rule(Rule *rule, char *cflags, FILE *make, char *filename);
int clean_rule(FILE *make, char *name);

// Constant array containing names of all standard C libraries excluding math.h
const char *standard_headers[] = {"stdio.h", "string.h", "assert.h", "complex.h",
"ctype.h", "errno.h", "fenv.h", "float.h", "inttypes.h", "iso646.h", "limits.h",
"locale.h", "setjmp.h", "signal.h", "stdalign.h", "stdarg.h", "stdatomic.h", "stdbool",
"stddef.h", "stdint.h", "stdlib.h", "stdnoreturn.h", "tgmath.h", "threads.h",
"uchar.h", "wchar.h", "wctype.h", "math.h"};

int main(int argc, char *argv[]) {

  char name[50]; // string for final .exe name
  char *argo[argc - 1]; // array of strings to hold .o versions of given .c files
  char cflags[120]; // string to hold desired CFlags
  Rule rule;
  FILE *make = fopen("Makefile", "w"); // File pointer to create/print to Makefile

  printf("Give name of final executable: "); // asking user for name of the final .exe
  scanf("%s", name); // scanning the value into the name var

  printf("Give Cflags: "); // asking user for desired CFlags
  fgets(cflags, 120, stdin); // first fgets to get to next line
  fgets(cflags, 120, stdin); // second fgets to read user input
  cflags[strlen(cflags) - 1] = '\0'; // getting rid of \n at the end of CFlags


  // getting array of .o strings from parsing the .c strings
  get_o_files(argv, argc, argo);

  // printing the exe and all rules
  print_exe_rule(name, cflags, argo, make, argc);

  for(int i = 0; i < argc - 1; i++) {
    init_rule(argo[i], &rule);
    get_deps(argv[i + 1], &rule);
    print_rule(&rule, cflags, make, argv[i + 1]);
  }

  clean_rule(make, name);

  /*for(int i = 0; i < argc - 1; i++) {
    print_o_rule(argo[i], argv[i + 1], make);
  }*/

  return 0;
}

int get_deps(char *filename, Rule *rule) {
  FILE *file = fopen(filename, "r");
  char *line = (char*)malloc(sizeof(char) * 100);
  char *header;
  int standard = -1;

  if(rule->deps == NULL) {
    add_dep(filename, rule);
  }

  while(fgets(line, 100, file) != NULL) {
    if(strstr(line, "#include") == NULL) {
      return 0;
    }
    header = strchr(line, '<');
    if(header == NULL) {
      header = strchr(line, '"');
    }
    header++;
    header[strlen(header) - 2] = '\0';

    for(int i = 0; i < 28; i++) {
      if(strcmp(header, standard_headers[i]) == 0) {
        standard = 0;
        break;
      }
    }
    if(standard != 0) {

      add_dep(header, rule);

      get_deps(header, rule);
    }
    standard = -1;
  }

  return 0;
}

int add_dep(char *name, Rule *rule) {
  struct Node *dep = (struct Node*)malloc(sizeof(struct Node));
  dep->name = (char*)malloc(strlen(name) + 1);
  dep->next = NULL;
  strcpy(dep->name, name);
  if(rule->deps == NULL) {
    rule->deps = dep;
  } else {
    dep->next = rule->deps;
    rule->deps = dep;
  }

  return 0;
}

int get_o_files(char *argv[], int argc, char *argo[]) {

  char current[50];

  for(int i = 0; i < argc - 1; i++) {
    strcpy(current, argv[i + 1]);
    current[strlen(current) - 1] = 'o';
    argo[i] = (char*)malloc(strlen(current) + 1);
    strcpy(argo[i], current);
  }

  return 0;
}

int init_rule(char *target, Rule *rule) {

  //Allocating space for the target string
  rule->target = (char*)malloc(strlen(target) + 1);
  strcpy(rule->target, target);

  //Initializing the dependency linked list to NULL
  rule->deps = NULL;

  return 0;
}

int print_exe_rule(char *target, char *cflags, char *argo[], FILE *make, int argc) {

  // printing the all rule
  fprintf(make, "all: %s\n\n%s:", target, target);

  // printing dependencies for the .exe rule
  for(int i = 0; i < argc - 1; i++) {
    fprintf(make, " %s", argo[i]);
  }

  //printing second line of the .exe rule
  fprintf(make, "\n\tgcc %s -o %s", cflags, target);
  for(int i = 0; i < argc - 1; i++) {
    fprintf(make, " %s", argo[i]);
  }

  // going to next rule
  fprintf(make, "\n\n");

  return 0;
}

int print_rule(Rule *rule, char *cflags, FILE *make, char *filename) {

  struct Node *dep;

  fprintf(make, "%s:", rule->target);
  dep = rule->deps;

  while(dep != NULL) {
    fprintf(make, " %s", dep->name);
    dep = dep->next;
  }

  fprintf(make, "\n\tgcc %s -c %s\n\n", cflags, filename);

  return 0;
}

int clean_rule(FILE *make, char *name) {
  fprintf(make, "clean:\n\trm *.o %s", name);
  return 0;
}
