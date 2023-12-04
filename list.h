#ifndef LIST_H
#define LIST_H

typedef struct Node_list {
  char* element;
  struct Node_list* next;
} Node_list;

void list(char* db_file, char** query, char* req);
void list_commands(char** query, Node_list** head, char* req);
Node_list* createList(char* data);
Node_list* LADD(Node_list* head, char* data);
Node_list* LINS(Node_list* head, char* data, int index, char* element);
Node_list* LDEL(Node_list* head, char* element);
Node_list* LREM(Node_list* head, int index, char* element);
Node_list* LDEL_VAL(Node_list* head, char* value, char* element);
int LGET(Node_list* head, char* element);
void write_list(char* filename, Node_list* head, char* struct_name,
                char* struct_type);

#endif