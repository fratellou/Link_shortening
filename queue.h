#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node_que {
  char *data;
  struct Node_que *next;
} Node_que;

typedef struct Queue {
  Node_que *head;
  Node_que *tail;
  int size;
} Queue;

void queue(char *db_file, char **query, char *req);
void queue_commands(char **query, Queue *queue, char *req);
void QPUSH(Queue *queue, char *element);
char *QPOP(Queue *queue);
void write_queue(char *filename, Queue *queue, char *struct_name,
                 char *struct_type);

#endif