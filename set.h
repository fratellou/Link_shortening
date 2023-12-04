#ifndef SET_H
#define SET_H

typedef struct Node_set {
  char *element;
  struct Node_set *next;
} Node_set;

typedef struct Set {
  Node_set **buckets;
  int size;
} Set;

void set(char *db_file, char **query, char *req);
void set_commands(char **query, Set *set, char *req);
Set *createSet(int size);
int set_calc(char *key);
char *SADD(Set *set, char *element);
char *SREM(Set *set, char *element);
int SISMEMBER(Set *set, char *element);
void write_set(char *filename, Set *set, char *struct_name, char *struct_type);
void free_set(Set *set);
#endif