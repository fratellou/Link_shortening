#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"

// This function implements the array operation
void array(char *db_file, char **query, char *req) {
  char **line = malloc(MAX_LEN * sizeof(char *));
  int size = 0;
  int isnt_empty = 0;

  Array *array = createArray(MAX_LEN);
  STRUCT(line, db_file, isnt_empty, query[1], size, "arr:");
  if (isnt_empty) {
    for (int i = 1; i < size; i++) {
      ARADD(array, atoi(line[i]));
    }
  }
  array_commands(query, array, req);
  write_array(db_file, array, query[1], "arr:");
  for (int i = 0; i < size; i++) {
    free(line[i]);
  }
  free(line);
  free(array->data);
  free(array);
}

// Function to perform array commands based on the query
void array_commands(char **query, Array *array, char *req) {
  if (!strcmp(query[0], "ARADD")) {
    sprintf(req, "%d", ARADD(array, atoi(query[2])));
  } else if (!strcmp(query[0], "ARINS")) {
    sprintf(req, "%d", ARINS(array, atoi(query[2]), atoi(query[3])));
  } else if (!strcmp(query[0], "ARDEL")) {
    sprintf(req, "%d", ARDEL(array));
  } else if (!strcmp(query[0], "ARREM")) {
    sprintf(req, "%d", ARREM(array, atoi(query[2])));
  } else if (!strcmp(query[0], "ARGET")) {
    sprintf(req, "%d", ARGET(array, atoi(query[2])));
  } else if (!strcmp(query[0], "ARSRCH")) {
    sprintf(req, "%d", ARSRCH(array, atoi(query[2])));
  } else if (!strcmp(query[0], "ARCHG")) {
    sprintf(req, "%d", ARCHG(array, atoi(query[2]), atoi(query[3])));
  } else {
    ERROR;
  }
}

// Function to create an array
Array *createArray(int size) {
  Array *array = (Array *)malloc(sizeof(Array));
  array->data = (int *)malloc(size * sizeof(int));
  array->size = 0;
  return array;
}

// Function to add an element to the end of the array
int ARADD(Array *arr, int element) {
  if (arr->size < MAX_LEN) {
    arr->data[arr->size++] = element;
    return element;
  }
  ERROR;
  return -1;
}

// Function to insert an element at a specific index in the array
int ARINS(Array *arr, int index, int element) {
  if (arr->size < MAX_LEN && index >= 0 && index <= arr->size) {
    for (int i = arr->size; i > index; i--) {
      arr->data[i] = arr->data[i - 1];
    }
    arr->data[index] = element;
    arr->size++;
    return element;
  }
  ERROR;
  return -1;
}

// Function to delete the last element in the array
int ARDEL(Array *arr) {
  if (arr->size > 0) {
    int index = arr->size;
    arr->size--;
    return arr->data[index - 1];
  }
  ERROR;
  return -1;
}

// Function to remove an element at a specific index in the array
int ARREM(Array *arr, int index) {
  if (arr->size > 0 && index >= 0 && index < arr->size) {
    int element = arr->data[index];
    for (int i = index; i < arr->size - 1; i++) {
      arr->data[i] = arr->data[i + 1];
    }
    arr->size--;
    return element;
  }
  ERROR;
  return -1;
}

// Function to get the value at a specific index in the array
int ARGET(Array *array, int index) {
  if (index >= 0 && index < array->size) {
    return array->data[index];
  }
  ERROR;
  return -1;
}

// Function to change the value at a specific index in the array
int ARCHG(Array *array, int index, int element) {
  if (index >= 0 && index < array->size) {
    array->data[index] = element;
    return element;
  }
  ERROR;
  return -1;
}

// Function to search for an element in the array and return its index
int ARSRCH(Array *array, int element) {
  for (int i = 0; i < array->size; i++) {
    if (array->data[i] == element) {
      return i;
    }
  }
  ERROR;
  return -1;
}

// Function to write the array data to a file
void write_array(char *filename, Array *arr, char *struct_name,
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
        for (int i = 0; i < arr->size; i++) {
          fprintf(temp, "%d ", arr->data[i]);
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
