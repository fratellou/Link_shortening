#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"

// This function is responsible for managing the operations on the tree
void tree(char *db_file, char **query, char *req) {
  char **line = malloc(MAX_LEN * sizeof(char *));
  int size = 0;
  int isnt_empty = 0;

  Node_tree *root = NULL;
  STRUCT(line, db_file, isnt_empty, query[1], size, "tree:");
  if (isnt_empty) {
    // Looping over the line array to insert the keys into the tree
    for (int i = 1; i < size; i++) {
      root = TADD(root, atoi(line[i]));
    }
  }
  tree_commands(query, &root, req);
  printTree(root, 0);
  // Writing the tree back to the database file
  write_tree(db_file, &root, query[1], "tree:");
  freeTree(root);
  for (int i = 0; i < size; i++) {
    free(line[i]);
  }
  free(line);
}

// This function handles the various commands that can be executed on the tree
void tree_commands(char **query, Node_tree **root, char *req) {
  if (!strcmp(query[0], "TADD")) {
    *root = TADD(*root, atoi(query[2]));
    strcpy(req, query[2]);
  } else if (!strcmp(query[0], "TSRCH")) {
    Node_tree *search = TSRCH(*root, atoi(query[2]));
    if (search != NULL) {
      strcpy(req, "TRUE");
    } else
      strcpy(req, "FALSE");
  } else if (!strcmp(query[0], "TDEL")) {
    *root = TDEL(*root, atoi(query[2]));
    strcpy(req, query[2]);
  } else {
    ERROR;
  }
}

// This function creates a new tree node with the given key
Node_tree *createTree(int key) {
  Node_tree *tmp = malloc(sizeof(Node_tree));
  tmp->key = key;
  tmp->parent = NULL;
  tmp->left = NULL;
  tmp->right = NULL;
  return tmp;
}

// This function inserts a new node with the given key into the tree
Node_tree *TADD(Node_tree *root, int key) {
  // If the tree is empty, create a new node and return it
  if (root == NULL) return createTree(key);

  Node_tree *root2 = root, *root3 = NULL;
  Node_tree *tmp = malloc(sizeof(Node_tree));
  tmp->key = key;
  tmp->parent = NULL;
  tmp->left = NULL;
  tmp->right = NULL;
  // Traversing the tree to find the appropriate position to insert the new node
  while (root2 != NULL) {
    root3 = root2;
    if (key < root2->key)
      root2 = root2->left;
    else
      root2 = root2->right;
  }
  tmp->parent = root3;
  if (key < root3->key)
    root3->left = tmp;
  else
    root3->right = tmp;

  return root;
}

// This function searches for a node in the tree with the given key
Node_tree *TSRCH(Node_tree *root, int data) {
  if (root == NULL || root->key == data) {
    return root;
  } else if (data < root->key) {
    return TSRCH(root->left, data);
  } else {
    return TSRCH(root->right, data);
  }
}

// This function returns the successor of a given node in the tree
Node_tree *succ(Node_tree *root) {
  Node_tree *p = root, *l = NULL;
  if (p->right != NULL) return min(p->right);
  l = p->parent;
  // Traversing up the tree to find the successor
  while ((l != NULL) && (p == l->right)) {
    p = l;
    l = l->parent;
  }
  return l;
}

// This function returns the minimum value node in the tree
Node_tree *min(Node_tree *root) {
  Node_tree *l = root;
  // Traversing down the left subtree to find the minimum value
  while (l->left != NULL) l = l->left;
  return l;
}

// Deletes a node with the specified data from a binary search tree
Node_tree *TDEL(Node_tree *root, int data) {
  Node_tree *l = NULL, *m = NULL;
  l = TSRCH(root, data);
  if (l != NULL) {
    if ((l->left == NULL) && (l->right == NULL)) {
      m = l->parent;
      if (l == m->right)
        m->right = NULL;
      else
        m->left = NULL;
    }

    if ((l->left == NULL) && (l->right != NULL)) {
      m = l->parent;
      if (l == m->right)
        m->right = l->right;
      else
        m->left = l->right;
    }

    if ((l->left != NULL) && (l->right == NULL)) {
      m = l->parent;
      if (l == m->right)
        m->right = l->left;
      else
        m->left = l->left;
    }

    if ((l->left != NULL) && (l->right != NULL)) {
      m = succ(l);
      l->key = m->key;
      if (m->right == NULL)
        m->parent->left = NULL;
      else
        m->parent->left = m->right;
    }
  }
  return root;
}

// Prints the contents of a binary tree in preorder traversal to a file
void printTree_in_file(Node_tree *root, FILE *file) {
  if (root != NULL) {
    if (root->key) {
      fprintf(file, "%d ", root->key);
    }
    printTree_in_file(root->left, file);
    printTree_in_file(root->right, file);
  }
}

// Prints the contents of a binary tree in a structured format
void printTree(Node_tree *tree, int space) {
  if (tree == NULL) {
    return;
  }
  space += 4;
  printTree(tree->right, space);
  printf("\n");
  for (int i = 4; i < space; i++) {
    printf(" ");
  }
  printf("%d\n", tree->key);
  printTree(tree->left, space);
}

// Frees the memory allocated for a binary tree
void freeTree(Node_tree *node) {
  if (node == NULL) {
    return;
  }

  freeTree(node->left);
  freeTree(node->right);

  free(node);
}

// Writes the contents of a binary tree to a file
void write_tree(char *filename, Node_tree **root, char *struct_name,
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
        printTree_in_file(*root, temp);
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
