// Node structure that represents a dependency. Contains the name of the
// dependency and a pointer to the next dependency in the list.
struct Node {
  char *name;
  struct Node *next;
};

// Structure defining a rule of the Makefile. Contains the rule target and
// a linked list of dependencies for that rule.
typedef struct Rule {
  char *target;
  struct Node *deps;
} Rule;
