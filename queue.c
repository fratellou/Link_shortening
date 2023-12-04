#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"

// This function implements the queue operation
void queue(char *db_file, char **query, char *req) {
  char **line = malloc(MAX_LEN * sizeof(char *));
  int isnt_empty = 0;
  int size = 0;
  Queue queue = {NULL, NULL, 0};
  STRUCT(line, db_file, isnt_empty, query[1], size, "queue:");
  if (isnt_empty) {
    for (int i = 1; i < size; i++) {
      QPUSH(&queue, line[i]);
    }
  }
  queue_commands(query, &queue, req);
  write_queue(db_file, &queue, query[1], "queue:");
  for (int i = 0; i < queue.size; i++) {
    free(line[i]);
  }
  free(line);
}

// Process the queue commands specified in the query
void queue_commands(char **query, Queue *queue, char *req) {
  if (!strcmp(query[0], "QPUSH")) {
    QPUSH(queue, query[2]);
    strcpy(req, query[2]);
  } else if (!strcmp(query[0], "QPOP")) {
    strcpy(req, QPOP(queue));
  } else
    ERROR;
}

// Push an element into the queue
void QPUSH(Queue *queue, char *element) {
  Node_que *node = malloc(sizeof(Node_que));
  node->data = strdup(element);

  // If the queue is empty, set the new node as both head and tail
  if (queue->head == NULL) {
    queue->head = node;
    queue->tail = node;
  } else {
    queue->tail->next = node;
    queue->tail = node;
  }
  queue->size++;
}

// Pop an element from the queue
char *QPOP(Queue *queue) {
  if (queue->head == NULL) {
    return NULL;
  } else {
    char *val = queue->head->data;
    queue->head = queue->head->next;
    queue->size--;
    return val;
  }
}

// Write the elements in the queue back to the database file
void write_queue(char *filename, Queue *queue, char *struct_name,
                 char *struct_type) {
  FILE *temp = fopen("temp.txt", "w+");
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
        for (int i = 0; i < queue->size; i++) {
          fprintf(temp, "%s ", queue->head->data);
          queue->head = queue->head->next;
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
