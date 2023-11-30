#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"

// This function implements the hash operation
void hash(char *db_file, char **query, char *req) {
  char **line = malloc(MAX_LEN * sizeof(char *));
  int isnt_empty = 0;
  int size = 0;
  HashTable *hashtable = createHashTable(MAX_LEN);
  STRUCT(line, db_file, isnt_empty, query[1], size, "hash:");
  if (isnt_empty) {
    for (int i = 0; i < size; i++) {
      char new_string[MAX_LEN];
      strcpy(new_string, line[i]);
      char *first_key = strtok(line[i], ",");
      char *second_val = strtok(NULL, ",");
      HSET(hashtable, first_key, second_val);
    }
  }
  hash_commands(query, hashtable, req);
  write_hash(db_file, hashtable, query[1], "hash:");
  free(line);
  destroyHashTable(hashtable);
}

// Function to perform hash commands
void hash_commands(char **query, HashTable *hash, char *req) {
  if (!strcmp(query[0], "HSET")) {
    strcpy(req, HSET(hash, query[2], query[3]));
  } else if (!strcmp(query[0], "HDEL")) {
    strcpy(req, HDEL(hash, query[2]));
  } else if (!strcmp(query[0], "HGET")) {
    strcpy(req, HGET(hash, query[2]));
  } else {
    ERROR;
  }
}

// Function to create a new hash table
HashTable *createHashTable(int size) {
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  ht->size = size;
  ht->table = malloc(sizeof(Node_hash **) * size);
  for (int i = 0; i < size; i++) {
    ht->table[i] = NULL;
  }
  return ht;
}

// Function to calculate the hash value for a given key
int hash_calc(char *key) {
  int hash = 0;
  for (int i = 0; i < (int)strlen(key); i++) {
    hash += (int)key[i];
  }
  return hash % MAX_LEN;
}

// Function to insert or update a key-value pair in the hash table
char *HSET(HashTable *hashtable, char *key, char *value) {
  int index = hash_calc(key) % hashtable->size;
  Node_hash *newNode = (Node_hash *)malloc(sizeof(Node_hash));
  newNode->key = key;
  newNode->element = value;
  newNode->next = NULL;
  if (hashtable->table[index] == NULL) {
    hashtable->table[index] = newNode;
  } else {
    Node_hash *current = hashtable->table[index];
    while (current->next != NULL) {
      if (strcmp(current->key, key) == 0) {
        current->element = value;
        return value;
      }
      current = current->next;
    }
    current->next = newNode;
  }
  return value;
}

// Function to delete a key-value pair from the hash table
char *HDEL(HashTable *hashtable, char *key) {
  int index = hash_calc(key);
  if (hashtable->table[index] == NULL) {
    return NULL;
  } else {
    Node_hash *current = hashtable->table[index];
    Node_hash *prev = NULL;
    while (current != NULL) {
      if (strcmp(current->key, key) == 0) {
        char *element = current->element;
        if (prev == NULL) {
          hashtable->table[index] = current->next;
        } else {
          prev->next = current->next;
        }
        return element;
      }
      prev = current;
      current = current->next;
    }
  }
  return NULL;
}

// Retrieves an element from the hash table based on the provided key
char *HGET(HashTable *hashtable, char *key) {
  int index = hash_calc(key);
  Node_hash *current = hashtable->table[index];
  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      return current->element;
    }
    current = current->next;
  }
  return NULL;
}

// Writes the contents of the hash table to a file
void write_hash(char *filename, HashTable *hashtable, char *struct_name,
                char *struct_type) {
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
        for (int i = 0; i < hashtable->size; i++) {
          Node_hash *current = hashtable->table[i];
          if (hashtable->table[i] == NULL || current->element == NULL ||
              current->key == NULL)
            continue;
          while (current != NULL) {
            fprintf(temp, "%s,%s ", current->key, current->element);
            current = current->next;
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

// Frees the memory used by the hash table and its elements
void destroyHashTable(HashTable *hashtable) {
  for (int i = 0; i < hashtable->size; i++) {
    Node_hash *current = hashtable->table[i];
    while (current != NULL) {
      Node_hash *temp = current;
      current = current->next;
      free(temp->key);
      free(temp);
    }
  }
  free(hashtable->table);
  free(hashtable);
}