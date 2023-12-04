#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"

// This function implements the list operation
void list(char* db_file, char** query, char* req) {
  char** line = malloc(MAX_LEN * sizeof(char*));
  int size = 0;
  int isnt_empty = 0;

  Node_list* head = NULL;
  STRUCT(line, db_file, isnt_empty, query[1], size, "list:");
  if (isnt_empty) {
    for (int i = size - 1; i > 0; i--) {
      head = LADD(head, line[i]);
    }
  }

  list_commands(query, &head, req);
  write_list(db_file, head, query[1], "list:");
  for (int i = 0; i < size; i++) {
    free(line[i]);
  }
  free(line);
}

// Executes the set commands based on the given query
void list_commands(char** query, Node_list** head, char* req) {
  if (!strcmp(query[0], "LADD")) {
    *head = LADD(*head, query[2]);
    strcpy(req, (*head)->element);
  } else if (!strcmp(query[0], "LINS")) {
    char* element = malloc(sizeof(char) * MAX_LEN);
    *head = LINS(*head, query[2], atoi(query[3]), element);
    strcpy(req, element);
    free(element);
  } else if (!strcmp(query[0], "LREM")) {
    char* element = malloc(sizeof(char) * MAX_LEN);
    *head = LREM(*head, atoi(query[2]), element);
    strcpy(req, element);
    free(element);
  } else if (!strcmp(query[0], "LDEL")) {
    char* element = malloc(sizeof(char) * MAX_LEN);
    *head = LDEL(*head, element);
    strcpy(req, element);
    free(element);
  } else if (!strcmp(query[0], "LDEL_VAL")) {
    char* element = malloc(sizeof(char) * MAX_LEN);
    *head = LDEL_VAL(*head, query[2], element);
    strcpy(req, element);
    free(element);
  } else if (!strcmp(query[0], "LGET")) {
    sprintf(req, "%d", LGET(*head, query[2]));
  } else if (!strcmp(query[0], "LISMEMBER")) {
    int index = LGET(*head, query[2]);
    if (index != -1)
      strcpy(req, "TRUE");
    else
      strcpy(req, "FALSE");
  } else {
    ERROR;
  }
}

// Creates and initializes a new list data structure
Node_list* createList(char* data) {
  Node_list* newNode = (Node_list*)malloc(sizeof(Node_list));
  newNode->element = data;
  newNode->next = NULL;
  return newNode;
}

// Function to add an element to the begin of the list
Node_list* LADD(Node_list* head, char* data) {
  Node_list* newNode = createList(data);
  if (head == NULL) {
    head = newNode;
  } else {
    newNode->next = head;
    head = newNode;
  }
  return head;
}

// Function for adding an element by index
Node_list* LINS(Node_list* head, char* data, int index, char* element) {
  Node_list* newNode = createList(data);
  if (head == NULL && index == 0) {
    strcpy(element, data);
    head = newNode;
  } else {
    Node_list* current = head;
    Node_list* previous = NULL;
    int count = 0;
    while (current != NULL && count < index) {
      previous = current;
      current = current->next;
      count++;
    }
    if (count == index) {
      strcpy(element, data);
      newNode->next = current;
      previous->next = newNode;
    } else
      strcpy(element, "n/a");
  }
  return head;
}

// Deletes an element from the list based on its value
Node_list* LDEL_VAL(Node_list* head, char* value, char* element) {
  Node_list *current = head, *prev = NULL;
  while (current != NULL) {
    if (strcmp(current->element, value) == 0) {
      strcpy(element, value);
      if (prev == NULL) {
        head = current->next;
      } else {
        prev->next = current->next;
      }
      break;
    }
    prev = current;
    current = current->next;
  }

  return head;
}

// Function to remove an item from the beginning of the list
Node_list* LDEL(Node_list* head, char* element) {
  if (head == NULL) {
    strcpy(element, "n/a");
    return NULL;
  } else {
    strcpy(element, head->element);
    head = head->next;
  }
  return head;
}

// Function for deleting an element by index
Node_list* LREM(Node_list* head, int index, char* element) {
  if (head == NULL) {
    strcpy(element, "n/a");
  } else {
    Node_list* current = head;
    Node_list* previous = NULL;
    int count = 0;
    while (current != NULL && count < index) {
      previous = current;
      current = current->next;
      count++;
    }
    if (count == index) {
      if (previous == NULL) {
        strcpy(element, head->element);
        head = current->next;
      } else {
        strcpy(element, current->element);
        previous->next = current->next;
      }
    } else
      strcpy(element, "n/a");
  }
  return head;
}

// Function to search for an item in the list
int LGET(Node_list* head, char* element) {
  Node_list* current = head;
  int index = 0;
  if (current == NULL) {
    return -1;
  }
  while (current != NULL && strcmp(current->element, element) != 0) {
    current = current->next;
    index++;
  }
  if (current == NULL) {
    return -1;
  } else {
    return index;
  }
}

// Writes the updated list data structure to the database file
void write_list(char* filename, Node_list* head, char* struct_name,
                char* struct_type) {
  FILE* temp = fopen("temp.txt", "a+");
  FILE* fp = fopen(filename, "r");
  if (fp && temp) {
    char string[MAX_LEN];
    int new_input = 0;
    while (fgets(string, MAX_LEN, fp) != NULL) {
      char new_string[MAX_LEN];
      strcpy(new_string, string);
      char* istr = strtok(string, " ");
      char* second_word = strtok(NULL, " ");
      if (new_input == 0) {
        fprintf(temp, "%s %s ", struct_type, struct_name);
        while (head != NULL) {
          fprintf(temp, "%s ", head->element);
          head = head->next;
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
