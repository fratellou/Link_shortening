#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "array.h"
#include "double_list.h"
#include "hash.h"
#include "list.h"
#include "macro.h"
#include "queue.h"
#include "stack.h"
#include "tree.h"
#include "set.h"

#define PORT 6379
pthread_mutex_t mutex;

void request(char *db_file, char **query, char *req);
void handle_client(int client_socket);

int main() {
  struct sockaddr_in server_address, client_address;

  // Creating a socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }

  // Setting up the server address
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(PORT);

  // Binding a socket to a server address
  if (bind(server_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) == -1) {
    perror("Failed to bind socket");
    exit(EXIT_FAILURE);
  }

  // Listening for connections
  if (listen(server_socket, 10) == -1) {
    perror("Failed to listen");
    exit(EXIT_FAILURE);
  }

  printf("Server is running and waiting for connections...\n");

  while (1) {
    socklen_t client_address_length = sizeof(client_address);
    int client_socket =
        accept(server_socket, (struct sockaddr *)&client_address,
               &client_address_length);
    if (client_socket < 0) {
      perror("Connection acceptance error");
      exit(EXIT_FAILURE);
    }

    printf("New connection accepted\n");

    // Processing a client request in a separate thread
    handle_client(client_socket);
  }

  close(server_socket);
  return 0;
}

void handle_client(int client_socket) {
  pthread_mutex_init(&mutex, NULL);
  char *db_file = "file.data";
  char *query = malloc(MAX_LEN * sizeof(char));
  if (read(client_socket, query, MAX_LEN) < 0) {
    perror("Error reading from socket");
    exit(EXIT_FAILURE);
  }
  char *req = malloc(MAX_LEN * sizeof(char));
  char **parsed_query = malloc(MAX_LEN * sizeof(char *));
  char *istr = strtok(query, " ");
  int i = 0;
  while (istr != NULL) {
    parsed_query[i] = malloc(MAX_LEN * sizeof(char));
    strcpy(parsed_query[i], istr);
    istr = strtok(NULL, " ");
    i++;
  }

  request(db_file, parsed_query, req);
  send(client_socket, req, 4, 0);
  free(query);
  free(req);
  pthread_mutex_destroy(&mutex);
  close(client_socket);

  printf("Connection completed\n");
}

void request(char *db_file, char **query, char *req) {
  pthread_mutex_lock(&mutex);
  if (!strcmp(query[0], "LADD") || !strcmp(query[0], "LINS") ||
      !strcmp(query[0], "LDEL") || !strcmp(query[0], "LREM") ||
      !strcmp(query[0], "LGET") || !strcmp(query[0], "LDEL_VAL") ||
      !strcmp(query[0], "LISMEMBER")) {
    list(db_file, query, req);
  } else if (!strcmp(query[0], "SPUSH") || !strcmp(query[0], "SPOP")) {
    stack(db_file, query, req);
  } else if (!strcmp(query[0], "QPUSH") || !strcmp(query[0], "QPOP")) {
    queue(db_file, query, req);
  } else if (!strcmp(query[0], "HSET") || !strcmp(query[0], "HDEL") ||
             !strcmp(query[0], "HGET")) {
    hash(db_file, query, req);
  } else if (!strcmp(query[0], "DLADD") || !strcmp(query[0], "DLINS") ||
             !strcmp(query[0], "DLDEL") || !strcmp(query[0], "DLREM") ||
             !strcmp(query[0], "DLGET") || !strcmp(query[0], "DLDEL_VAL") ||
             !strcmp(query[0], "DLREMR") || !strcmp(query[0], "DLISMEMBER") ||
             !strcmp(query[0], "DLADDR")) {
    Dlist(db_file, query, req);
  } else if (!strcmp(query[0], "ARADD") || !strcmp(query[0], "ARREM") ||
             !strcmp(query[0], "ARDEL") || !strcmp(query[0], "ARINS") ||
             !strcmp(query[0], "ARGET") || !strcmp(query[0], "ARSRCH") ||
             !strcmp(query[0], "ARCHG")) {
    array(db_file, query, req);
  } else if (!strcmp(query[0], "TADD") || !strcmp(query[0], "TSRCH") ||
             !strcmp(query[0], "TDEL")) {
    tree(db_file, query, req);
  } else if (!strcmp(query[0], "SADD") || !strcmp(query[0], "SREM") ||
             !strcmp(query[0], "SISMEMBER")) {
    set(db_file, query, req);
  } else
    ERROR;
  pthread_mutex_unlock(&mutex);
}
