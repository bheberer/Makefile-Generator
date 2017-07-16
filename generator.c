#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rule_struct.h"

int get_o_files(char *argv[], int argc, char *o_files[]);
int init_rule(char *target, Rule *rule);
int print_exe_rule(char *target, char *cflags, char *o_files[], FILE *make, int argc);
int get_deps(char *filename, Rule *rule);
int add_dep(char *name, Rule *rule);
int print_rule(Rule *rule, char *cflags, FILE *make, char *filename);
int print_clean_rule(FILE *make, char *exe_name);
int check_cmds(char *argv[], int argc);
int free_rule(Rule *rule);
int check_duplicate(char *name, Rule *rule);

// Constant array containing names of all standard C libraries
const char *standard_headers[] = {"stdio.h", "string.h", "assert.h", "complex.h",
"ctype.h", "errno.h", "fenv.h", "float.h", "inttypes.h", "iso646.h", "limits.h",
"locale.h", "setjmp.h", "signal.h", "stdalign.h", "stdarg.h", "stdatomic.h", "stdbool",
"stddef.h", "stdint.h", "stdlib.h", "stdnoreturn.h", "tgmath.h", "threads.h",
"uchar.h", "wchar.h", "wctype.h", "math.h"};

int main(int argc, char *argv[]) {

  char exe_name[50];
  char *o_files[argc - 1];
  char cflags[120];
  Rule rule;
  FILE *make = fopen("Makefile", "w");

  // Checks to see if the commands provided are valid
  if(check_cmds(argv, argc) == -1) {
    return 0;
  }

  // Getting desired name for the final exe file
  printf("Give name of final executable: ");
  scanf("%s", exe_name);

  // Getting desired cflags
  printf("Give Cflags: ");
  fgets(cflags, 120, stdin);
  fgets(cflags, 120, stdin);
  cflags[strlen(cflags) - 1] = '\0';

  get_o_files(argv, argc, o_files);

  print_exe_rule(exe_name, cflags, o_files, make, argc);

  // Initializing object file , finding all of their dependencies and
  // printing them to the Makefile.
  for(int i = 0; i < argc - 1; i++) {
    init_rule(o_files[i], &rule);
    get_deps(argv[i + 1], &rule);
    print_rule(&rule, cflags, make, argv[i + 1]);
    free_rule(&rule);
  }

  print_clean_rule(make, exe_name);

  return 0;
}

// Recursive function that takes in the name of a file and reads through said
// file, looking for dependencies. The function then performs a recursive call
// on found dependencies looking for their dependencies. All found dependencies
// are added to the rule parameter.
int get_deps(char *filename, Rule *rule) {

  FILE *file = fopen(filename, "r");
  int hit_include = -1;

  if(file == NULL) {
    printf("Error: %s does not exist", filename);
  }

  char *line = (char*)malloc(sizeof(char) * 100);
  char *header;
  int standard = -1;

  // Adding the source file dependency
  if(rule->deps == NULL) {
    add_dep(filename, rule);
  }

  // Loop to read through each line of the file
  while(fgets(line, 100, file) != NULL) {

    // When an include line is found, hit_include is set to 0. When a line
    // comes after the includes, hit_include is used to break the loop. If a
    // line comes before the includes, they are skipped over.
    if(strstr(line, "#include") != NULL) {
      hit_include = 0;
    } else if(strstr(line, "#include") == NULL && hit_include == 0) {
      break;
    } else {
      continue;
    }

    // The header filename is parsed out of the read line
    header = strchr(line, '<');
    if(header == NULL) {
      header = strchr(line, '"');
    }
    header++;
    header[strlen(header) - 2] = '\0';

    // checking to see if the dependency alread exists
    if(check_duplicate(header, rule) == -1) {
      continue;
    }

    // The header is compared to the standard library for C
    for(int i = 0; i < 28; i++) {
      if(strcmp(header, standard_headers[i]) == 0) {
        standard = 0;
        break;
      }
    }

    // If the header isn't part of the standard library, it gets added to the
    // rule, and a recursive call is made to see if it has any other dependencies
    if(standard != 0) {
      add_dep(header, rule);
      get_deps(header, rule);
    }
    standard = -1;
  }

  return 0;
}

// Function that adds a dependency of the given name to the dependency linked
// list located in the provided rule.
int add_dep(char *name, Rule *rule) {

  struct Node *dep = (struct Node*)malloc(sizeof(struct Node));
  dep->name = (char*)malloc(strlen(name) + 1);
  dep->next = NULL;
  strcpy(dep->name, name);

  // The new dependency always gets put to the head position of rule->deps
  if(rule->deps == NULL) {
    rule->deps = dep;
  } else {
    dep->next = rule->deps;
    rule->deps = dep;
  }

  return 0;
}

// This function takes the given command line arguments and turns the .c files
// given by the user into .o files that are put into the o_files output parameter
int get_o_files(char *argv[], int argc, char *o_files[]) {

  char current[50];

  for(int i = 0; i < argc - 1; i++) {
    strcpy(current, argv[i + 1]);
    current[strlen(current) - 1] = 'o';
    o_files[i] = (char*)malloc(strlen(current) + 1);
    strcpy(o_files[i], current);
  }

  return 0;
}

// Function that allocates space for the rule of the given target
int init_rule(char *target, Rule *rule) {

  //Allocating space for the target string
  rule->target = (char*)malloc(strlen(target) + 1);
  strcpy(rule->target, target);

  //Initializing the dependency linked list to NULL
  rule->deps = NULL;

  return 0;
}

// Function that takes in the desired name of the exe, the desired cflags, and the
// required .o files in order to print out the first rule to the make output
// parameter
int print_exe_rule(char *target, char *cflags, char *o_files[], FILE *make, int argc) {

  // printing the all rule
  fprintf(make, "all: %s\n\n%s:", target, target);

  // printing dependencies for the .exe rule
  for(int i = 0; i < argc - 1; i++) {
    fprintf(make, " %s", o_files[i]);
  }

  //printing second line of the .exe rule
  fprintf(make, "\n\tgcc %s -o %s", cflags, target);
  for(int i = 0; i < argc - 1; i++) {
    fprintf(make, " %s", o_files[i]);
  }

  // going to next rule
  fprintf(make, "\n\n");

  return 0;
}

// Function that when given a rule, the desired cflags and the appropriate filename,
// prints out said rule with proper makefile formatting to the make output parameter
int print_rule(Rule *rule, char *cflags, FILE *make, char *filename) {

  struct Node *dep;

  // Printing the first line
  fprintf(make, "%s:", rule->target);
  dep = rule->deps;
  while(dep != NULL) {
    fprintf(make, " %s", dep->name);
    dep = dep->next;
  }

  // printing the second line
  fprintf(make, "\n\tgcc %s -c %s\n\n", cflags, filename);

  return 0;
}

// Function that takes in the exe_name and prints out the clean rule to the
// make output parameter. By default this deletes all .o files and the exe file
int print_clean_rule(FILE *make, char *exe_name) {
  fprintf(make, "clean:\n\trm *.o %s", exe_name);
  return 0;
}

// Function that takes in the command line arguments and checks to see that
// the given source files are actually .c files
int check_cmds(char *argv[], int argc) {

  // checking to see if the arguments have ".c" in them
  for(int i = 1; i < argc; i++) {
    if(strstr(argv[i], ".c") == NULL) {
      printf("Error: Please give .c source files only");
      return -1;
    }
  }

  return 0;
}

// Function that frees all of the memory of the provided rule
int free_rule(Rule *rule) {

  struct Node *temp, *curr;

  // temp node is used to help free all of the dependencies
  while(curr != NULL) {
    temp = curr->next;
    free(curr->name);
    free(curr);
    curr = temp;
  }

  // rule target free and rule set to NULL
  rule->deps = NULL;
  free(rule->target);
  rule = NULL;

  return 0;
}

// Function that takes the name of a dependency and a rule. It checks to see
// if a dependency with that name already exists, and returns -1 if this it
// does, and 0 if it does not
int check_duplicate(char *name, Rule *rule) {

  struct Node *curr;

  curr = rule->deps;

  while(curr != NULL) {
    if(strcmp(curr->name, name) == 0) {
      return -1;
    }
    curr = curr->next;
  }

  return 0;
}
