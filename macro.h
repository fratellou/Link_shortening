#ifndef MACRO_H
#define MACRO_H

#define MAX_LEN 256
#ifndef ERROR
#define ERROR printf("n/a\n");
#endif

#ifndef STRUCT
#define STRUCT(line, filename, isnt_empty, struct_name, size, struct_type) \
  {                                                                        \
    FILE *fp = fopen(filename, "r");                                       \
    if (fp == NULL) {                                                      \
      FILE *new = fopen(filename, "a+");                                   \
      fprintf(new, "\n");                                                  \
      ERROR;                                                               \
      fclose(new);                                                         \
    } else {                                                               \
      fclose(fp);                                                          \
      FILE *file = fopen(filename, "r");                                   \
      if (file) {                                                          \
        char *string = malloc(MAX_LEN * sizeof(char));                     \
        while (fgets(string, MAX_LEN, file) != NULL) {                     \
          char *istr = strtok(string, " ");                                \
          char *name = strtok(NULL, " ");                                  \
          if ((strcmp(istr, struct_type) == 0) &&                          \
              (strcmp(struct_name, name) == 0)) {                          \
            while (istr != NULL) {                                         \
              if (strcmp(istr, "\n") != 0) {                               \
                line[size] = malloc(MAX_LEN * sizeof(char));               \
                strcpy(line[size], istr);                                  \
                size++;                                                    \
              }                                                            \
              istr = strtok(NULL, " ");                                    \
            }                                                              \
            isnt_empty = 1;                                                \
            break;                                                         \
          }                                                                \
        }                                                                  \
        free(string);                                                      \
        fclose(file);                                                      \
      } else {                                                             \
        ERROR;                                                             \
      }                                                                    \
    }                                                                      \
  }
#endif

#endif
