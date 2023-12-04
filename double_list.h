#ifndef DLIST_H
#define DLIST_H

typedef struct Node_Dlist {
  char* element;
  struct Node_Dlist* prev;
  struct Node_Dlist* next;
} Node_Dlist;

typedef struct DList {
  Node_Dlist* head;
  Node_Dlist* tail;
} DList;

void Dlist(char* db_file, char** query, char* req);
void Dlist_commands(char** query, DList** list, char* req);
DList* createDList();
Node_Dlist* createNode(char* data);
DList* DLADD(DList* list, char* data);
DList* DLDEL_VAL(DList* list, char* value, char* element);
DList* DLINS(DList* list, char* data, int index, char* element);
Node_Dlist* DLDEL(Node_Dlist* head, char* element);
Node_Dlist* DLREM(Node_Dlist* head, int index, char* element);
DList* DLADDR(DList* list, char* data);
Node_Dlist* DLREMR(Node_Dlist* list, char* element);
int DLGET(Node_Dlist* head, char* element);
void write_Dlist(char* filename, Node_Dlist* head, char* struct_name,
                 char* struct_type);

#endif