#include "double_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"

// This function implements the list operation
void Dlist(char* db_file, char** query, char* req) {
  char** line = malloc(MAX_LEN * sizeof(char*));
  int size = 0;
  int isnt_empty = 0;

  DList* list = createDList();
  STRUCT(line, db_file, isnt_empty, query[1], size, "Dlist:");
  if (isnt_empty) {
    for (int i = size - 1; i > 0; i--) {
      list = DLADD(list, line[i]);
    }
  }

  Dlist_commands(query, &list, req);
  write_Dlist(db_file, list->head, query[1], "Dlist:");
  for (int i = 0; i < size; i++) {
    free(line[i]);
  }
  free(line);
}

// Executes the set commands based on the given query
void Dlist_commands(char** query, DList** list, char* req) {
  if (!strcmp(query[0], "DLADD")) {
    *list = DLADD(*list, query[2]);
    strcpy(req, (*list)->head->element);
  } else if (!strcmp(query[0], "DLINS")) {
    char* element = malloc(sizeof(char) * MAX_LEN);
    *list = DLINS(*list, query[2], atoi(query[3]), element);
    strcpy(req, element);
    free(element);
  } else if (!strcmp(query[0], "DLADDR")) {
    *list = DLADDR(*list, query[2]);
    strcpy(req, (*list)->tail->element);
  } else if (!strcmp(query[0], "DLREM")) {
    char* element = malloc(sizeof(char) * MAX_LEN);
    (*list)->head = DLREM((*list)->head, atoi(query[2]), element);
    strcpy(req, element);
    free(element);
  } else if (!strcmp(query[0], "DLDEL")) {
    char* element = malloc(sizeof(char) * MAX_LEN);
    (*list)->head = DLDEL((*list)->head, element);
    strcpy(req, element);
    free(element);
  } else if (!strcmp(query[0], "DLGET")) {
    sprintf(req, "%d", DLGET((*list)->head, query[2]));
  } else if (!strcmp(query[0], "DLDEL_VAL")) {
    char* element = malloc(sizeof(char) * MAX_LEN);
    *list = DLDEL_VAL(*list, query[2], element);
    strcpy(req, element);
    free(element);
  } else if (!strcmp(query[0], "DLREMR")) {
    char* element = malloc(sizeof(char) * MAX_LEN);
    (*list)->tail = DLREMR((*list)->tail, element);
    strcpy(req, element);
    free(element);
  } else if (!strcmp(query[0], "DLISMEMBER")) {
    int index = DLGET((*list)->head, query[2]);
    if (index != -1)
      strcpy(req, "TRUE");
    else
      strcpy(req, "FALSE");
  } else {
    ERROR;
  }
}

// Creates and initializes a new list data structure
DList* createDList() {
  DList* list = (DList*)malloc(sizeof(DList));
  list->head = NULL;
  list->tail = NULL;
  return list;
}

Node_Dlist* createNode(char* data) {
  Node_Dlist* newNode = (Node_Dlist*)malloc(sizeof(Node_Dlist));
  newNode->element = data;
  newNode->prev = NULL;
  newNode->next = NULL;
  return newNode;
}

// Function to add an element to the begin of the list
DList* DLADD(DList* list, char* data) {
  Node_Dlist* newNode = createNode(data);
  if (list->head == NULL) {
    list->head = newNode;
    list->tail = newNode;
  } else {
    newNode->next = list->head;
    list->head->prev = newNode;
    list->head = newNode;
  }
  return list;
}

// Function to add an element to the end of the list
DList* DLADDR(DList* list, char* data) {
  Node_Dlist* newNode = createNode(data);
  if (list->tail == NULL) {
    list->head = newNode;
    list->tail = newNode;
  } else {
    list->tail->next = newNode;
    newNode->prev = list->tail;
    list->tail = newNode;
  }
  return list;
}

// Removes given value from the doubly linked list and returns the updated node
DList* DLDEL_VAL(DList* list, char* value, char* element) {
  int index = DLGET(list->head, value);
  list->head = DLREM(list->head, index, element);
  return list;
}

// Function for adding an element by index
DList* DLINS(DList* list, char* data, int index, char* element) {
  Node_Dlist* newNode = createNode(data);
  if (list->head == NULL && index == 0) {
    strcpy(element, data);
    list->head = newNode;
    list->tail = newNode;
  } else {
    Node_Dlist* current = list->head;
    Node_Dlist* previous = NULL;
    int count = 0;
    while (current != NULL && count < index) {
      previous = current;
      current = current->next;
      count++;
    }
    if (count == index) {
      strcpy(element, data);
      newNode->next = current;
      newNode->prev = previous;
      if (current != NULL) {
        current->prev = newNode;
      } else {
        list->tail = newNode;
      }
      if (previous != NULL) {
        previous->next = newNode;
      } else {
        list->head = newNode;
      }
    } else {
      strcpy(element, "n/a");
    }
  }
  return list;
}

// Removes a node from the end of the doubly linked list
Node_Dlist* DLREMR(Node_Dlist* tail, char* element) {
  if (tail == NULL) {
    strcpy(element, "n/a");
    return NULL;
  } else {
    strcpy(element, tail->element);
    Node_Dlist* nextNode = tail->next;
    if (nextNode != NULL) {
      nextNode->prev = NULL;
      tail->next = NULL;
      nextNode->prev = nextNode;
    }
    tail = nextNode;
  }
  return tail;
}

// Function to remove an item from the beginning of the list
Node_Dlist* DLDEL(Node_Dlist* head, char* element) {
  if (head == NULL) {
    strcpy(element, "n/a");
    return NULL;
  } else {
    strcpy(element, head->element);
    Node_Dlist* nextNode = head->next;
    if (nextNode != NULL) {
      nextNode->prev = NULL;
      head->next = NULL;
      nextNode->prev = nextNode;
    }
    head = nextNode;
  }
  return head;
}

// Function for deleting an element by index
Node_Dlist* DLREM(Node_Dlist* head, int index, char* element) {
  if (head == NULL) {
    strcpy(element, "n/a");
  } else {
    Node_Dlist* current = head;
    Node_Dlist* previous = NULL;
    int count = 0;
    while (current != NULL && count < index) {
      previous = current;
      current = current->next;
      count++;
    }
    if (count == index) {
      strcpy(element, current->element);
      if (previous == NULL) {
        head = current->next;
        if (head != NULL) {
          head->prev = NULL;
        }
      } else {
        previous->next = current->next;
        if (current->next != NULL) {
          current->next->prev = previous;
        }
      }
    } else {
      strcpy(element, "n/a");
    }
  }
  return head;
}

// Function to search for an item in the list
int DLGET(Node_Dlist* head, char* element) {
  Node_Dlist* current = head;
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
void write_Dlist(char* filename, Node_Dlist* head, char* struct_name,
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
