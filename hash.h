#ifndef HASH_H
#define HASH_H

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

void hash(char *db_file, char **query, char *req);
void hash_commands(char **query, HashTable *hash, char *req);
HashTable *createHashTable(int size);
int hash_calc(char *key);
char *HSET(HashTable *hashtable, char *key, char *value);
char *HDEL(HashTable *hashtable, char *key);
char *HGET(HashTable *hashtable, char *key);
void write_hash(char *filename, HashTable *hashtable, char *struct_name,
                char *struct_type);
void destroyHashTable(HashTable *hashtable);

#endif