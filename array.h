#ifndef ARRAY_H
#define ARRAY_H

typedef struct {
  int *data;
  int size;
} Array;

void array(char *db_file, char **query, char *req);
void array_commands(char **query, Array *array, char *req);
Array *createArray(int size);
int ARADD(Array *arr, int element);
int ARINS(Array *arr, int index, int element);
int ARDEL(Array *arr);
int ARREM(Array *arr, int index);
int ARGET(Array *array, int index);
int ARCHG(Array *array, int index, int element);
int ARSRCH(Array *array, int element);
void write_array(char *filename, Array *arr, char *struct_name,
                 char *struct_type);

#endif