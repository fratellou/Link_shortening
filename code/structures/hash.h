#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct_reader.h"

typedef struct Node_hash {
  char *element;
  char *key;
  struct Node_hash *next;
} Node_hash;

typedef struct HashTable {
  Node_hash **table;
  int size;
  int capacity;
} HashTable;

void hash(char *db_file, char **query, char *req); // This function implements the hash operation
void hash_commands(char **query, HashTable *hash, char *req); // Function to perform hash commands
HashTable *createHashTable(int size); // Function to create a new hash table
int hash_calc(char *key); // Function to calculate the hash value for a given key
char *HSET(HashTable *hashtable, char *key, char *value); // Function to insert or update a key-value pair in the hash table
char *HDEL(HashTable *hashtable, char *key); // Function to delete a key-value pair from the hash table
char *HGET(HashTable *hashtable, char *key); // Retrieves an element from the hash table based on the provided key
void write_hash(char *filename, HashTable *hashtable, char *struct_name,
                char *struct_type); // Writes the contents of the hash table to a file
void destroyHashTable(HashTable *hashtable); // Frees the memory used by the hash table and its elements

#endif