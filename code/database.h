#ifndef DATABASE_H
#define DATABASE_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "./structures/hash.h"

#define PORT 6379
pthread_mutex_t mutex;

void request(char *db_file, char **query, char *req);
void *handle_client(void *arg);

#endif