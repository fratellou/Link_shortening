#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"

// This function implements the stack operation
void stack(char *db_file, char **query, char *request) {
  char **line = malloc(MAX_LEN * sizeof(char *));
  int isnt_empty = 0;
  int size = 0;
  Stack stack = {NULL, 0};
  STRUCT(line, db_file, isnt_empty, query[1], size, "stack:");
  if (isnt_empty) {
    for (int i = size - 1; i > 0; i--) {
      SPUSH(&stack, line[i]);
    }
  }
  stack_commands(query, &stack, request);
  // Write the stack elements back to the database file
  write_stack(db_file, &stack, query[1], "stack:");
  for (int i = 0; i < stack.size; i++) {
    free(line[i]);
  }
  free(line);
}

// This function performs stack commands based on the query
void stack_commands(char **query, Stack *stack, char *request) {
  if (!strcmp(query[0], "SPUSH")) {
    SPUSH(stack, query[2]);
    strcpy(request, query[2]);
  } else if (!strcmp(query[0], "SPOP")) {
    strcpy(request, SPOP(stack));
  } else
    ERROR;
}

// This function pushes an element onto the stack
void SPUSH(Stack *stack, char *element) {
  Node *node = malloc(sizeof(Node));
  node->data = strdup(element);
  if (stack->head == NULL) {
    stack->head = node;
  } else {
    node->next = stack->head;
    stack->head = node;
  }
  stack->size++;
}

// This function pops an element from the stack
char *SPOP(Stack *stack) {
  if (stack->head == NULL) {
    return NULL;
  } else {
    char *element = stack->head->data;
    stack->head = stack->head->next;
    stack->size--;
    return element;
  }
}

// This function writes the stack elements back to the database file
void write_stack(char *filename, Stack *stack, char *struct_name,
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
        for (int i = 0; i < stack->size; i++) {
          fprintf(temp, "%s ", stack->head->data);
          stack->head = stack->head->next;
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
