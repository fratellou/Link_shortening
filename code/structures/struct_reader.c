#include "struct_reader.h"

void read_struct_from_file(char **line, char *filename, int *isnt_empty, char *struct_name, int *size, char *struct_type) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        FILE *new = fopen(filename, "a+");
        fprintf(new, "\n");
        printf("Error: File not found\n");
        fclose(new);
    } else {
        fclose(fp);
        FILE *file = fopen(filename, "r");
        if (file) {
            char string[MAX_LEN];
            while (fgets(string, MAX_LEN, file) != NULL) {
                char *istr = strtok(string, " ");
                char *name = strtok(NULL, " ");
                if ((strcmp(istr, struct_type) == 0) && (strcmp(struct_name, name) == 0)) {
                    while (istr != NULL) {
                        if (strcmp(istr, "\n") != 0) {
                            line[*size] = malloc(MAX_LEN * sizeof(char));
                            strcpy(line[*size], istr);
                            (*size)++;
                        }
                        istr = strtok(NULL, " ");
                    }
                    *isnt_empty = 1;
                    break;
                }
            }
            fclose(file);
        } else {
            printf("Error: File open failed\n");
        }
    }
}