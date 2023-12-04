#ifndef TREE_H
#define TREE_H

#include <stdio.h>

typedef struct Node_tree {
  int key;
  struct Node_tree *left;
  struct Node_tree *right;
  struct Node_tree *parent;
} Node_tree;

void tree(char *db_file, char **query, char *req);
void tree_commands(char **query, Node_tree **root, char *req);
Node_tree *createTree(int key);
Node_tree *TADD(Node_tree *root, int key);
Node_tree *TSRCH(Node_tree *root, int data);
Node_tree *succ(Node_tree *root);
Node_tree *min(Node_tree *root);
Node_tree *TDEL(Node_tree *root, int data);
void printTree(Node_tree *tree, int space);
void printTree_in_file(Node_tree *root, FILE *file);
void freeTree(Node_tree *node);
void write_tree(char *filename, Node_tree **root, char *struct_name,
                char *struct_type);

#endif