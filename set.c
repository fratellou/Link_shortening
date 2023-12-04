#include "set.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"

// This function implements the set operation
void set(char *db_file, char **query, char *req) {
  char **line = malloc(MAX_LEN * sizeof(char *));
  int size = 0;
  int isnt_empty = 0;

  Set *set = createSet(MAX_LEN);
  STRUCT(line, db_file, isnt_empty, query[1], size, "set:");
  if (isnt_empty) {
    for (int i = 1; i < size; i++) {
      SADD(set, line[i]);
    }
  }
  set_commands(query, set, req);
  write_set(db_file, set, query[1], "set:");
  free_set(set);
  for (int i = 0; i < size; i++) {
    free(line[i]);
  }
  free(line);
}

// Executes the set commands based on the given query
void set_commands(char **query, Set *set, char *req) {
  if (!strcmp(query[0], "SADD")) {
    strcpy(req, SADD(set, query[2]));
  } else if (!strcmp(query[0], "SREM")) {
    strcpy(req, SREM(set, query[2]));
  } else if (!strcmp(query[0], "SISMEMBER")) {
    if (!SISMEMBER(set, query[2]))
      strcpy(req, "FALSE");
    else
      strcpy(req, "TRUE");
  } else {
    ERROR;
  }
}

// Creates and initializes a new set data structure
Set *createSet(int size) {
  Set *set = (Set *)malloc(sizeof(Set));
  set->size = size;
  set->buckets = malloc(sizeof(Node_set **) * size);
  for (int i = 0; i < size; i++) {
    set->buckets[i] = NULL;
  }
  return set;
}

// Calculates the hash value for a given key
int set_calc(char *key) {
  int hash = 0;
  for (int i = 0; i < (int)strlen(key); i++) {
    hash += (int)key[i];
  }
  return hash % MAX_LEN;
}

// Adds an element to the set
char *SADD(Set *set, char *element) {
  int index = set_calc(element);
  if (set->buckets[index] != NULL) {
    ERROR;
    return NULL;
  }
  Node_set *newNode = (Node_set *)malloc(sizeof(Node_set));
  newNode->element = element;
  newNode->next = set->buckets[index];
  set->buckets[index] = newNode;
  return element;
}

// Removes an element from the set
char *SREM(Set *set, char *element) {
  int index = set_calc(element);
  Node_set *current = set->buckets[index];
  Node_set *previous = NULL;
  while (current != NULL) {
    if (strcmp(current->element, element) == 0) {
      if (previous != NULL) {
        previous->next = current->next;
      } else {
        set->buckets[index] = current->next;
      }
      char *element = current->element;
      free(current);
      return element;
    }
    previous = current;
    current = current->next;
  }
  return NULL;
}

// Checks if an element is a member of the set
int SISMEMBER(Set *set, char *element) {
  int index = set_calc(element);
  Node_set *current = set->buckets[index];
  while (current != NULL) {
    if (strcmp(current->element, element) == 0) {
      return 1;
    }
    current = current->next;
  }

  return 0;
}

// Writes the updated set data structure to the database file
void write_set(char *filename, Set *set, char *struct_name, char *struct_type) {
  FILE *temp = fopen("temp.txt", "a+");
  FILE *fp = fopen(filename, "r");
  if (fp && temp) {
    char string[MAX_LEN];
    int new_input = 0;
    while (fgets(string, MAX_LEN, fp) != NULL) {
      char new_string[MAX_LEN];
      strcpy(new_string, string);
      char *istr = strtok(string, " ");
      char *second_word = strtok(NULL, " ");
      if (new_input == 0) {
        fprintf(temp, "%s %s ", struct_type, struct_name);
        for (int i = 0; i < MAX_LEN; i++) {
          if (set->buckets[i] != NULL) {
            fprintf(temp, "%s ", set->buckets[i]->element);
          }
        }
        fprintf(temp, "\n");
        new_input = 1;
      }
      if ((strcmp(istr, struct_type) == 0) &&
          (strcmp(second_word, struct_name) == 0)) {
        continue;
      } else {
        fprintf(temp, "%s", new_string);
      }
    }
    remove(filename);
    rename("temp.txt", filename);
  } else {
    ERROR;
  }
  fclose(fp);
  fclose(temp);
}

// Frees the memory allocated for the set data structure
void free_set(Set *set) {
  if (set == NULL) {
    return;
  }
  for (int i = 0; i < set->size; i++) {
    if (set->buckets[i] != NULL) {
      free(set->buckets[i]);
    }
  }
  free(set->buckets);
  free(set);
}