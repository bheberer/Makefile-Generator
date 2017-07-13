struct Node {
  char *name;
  struct Node *next;
};

typedef struct Rule {
  char *target;
  struct Node *deps;
} Rule;
