#ifndef MACRO_H
#define MACRO_H

#define MAX_LEN 2564

#ifndef ERROR
#define ERROR printf("n/a\n");
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_struct_from_file(char **line, char *filename, int *isnt_empty, char *struct_name, int *size, char *struct_type);

#endif
