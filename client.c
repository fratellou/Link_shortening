#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "macro.h"

#define PORT 6379
#define TASK_FLAGS 2

int main(int argc, char *argv[]) {
  if (argc > 0) {
    int counter_of_flags = 0;
    char *db_file = malloc(MAX_LEN * sizeof(char));
    char *query = malloc(MAX_LEN * sizeof(char));
    for (int i = 1; i < argc; i++) {
      if (!strcmp(argv[i], "--file")) {
        i++;
        strcpy(db_file, argv[i]);
        counter_of_flags++;

      } else if (!strcmp(argv[i], "--query")) {
        i++;
        strcpy(query, argv[i]);
        counter_of_flags++;
      } else {
        break;
      }
    }
    if (counter_of_flags == TASK_FLAGS && argc > (TASK_FLAGS)) {
      // Creating a socket
      int client_socket = socket(AF_INET, SOCK_STREAM, 0);
      if (client_socket == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
      }

      // Setting up the server address
      struct sockaddr_in server_address;
      server_address.sin_family = AF_INET;
      server_address.sin_addr.s_addr = INADDR_ANY;
      server_address.sin_port = htons(PORT);

      if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid server address");
        exit(EXIT_FAILURE);
      }

      // Connecting to the server
      if (connect(client_socket, (struct sockaddr *)&server_address,
                  sizeof(server_address)) == -1) {
        perror("Error connecting to the server");
        exit(EXIT_FAILURE);
      }

      // Sending a request
      if (send(client_socket, db_file, MAX_LEN, 0) == -1) {
        perror("Error when sending a message to the server");
        exit(EXIT_FAILURE);
      }

      if (send(client_socket, query, MAX_LEN, 0) == -1) {
        perror("Error when sending a message to the server");
        exit(EXIT_FAILURE);
      }

      printf("The request has been sent\n");
      char response[MAX_LEN];
      if (recv(client_socket, response, sizeof(response), 0) == -1) {
        perror("Error receiving response from server");
        exit(EXIT_FAILURE);
      }
      printf("REQUEST: %s\n", response);

      close(client_socket);
    }

    else {
      ERROR;
    }
    free(db_file);
    free(query);
  } else {
    ERROR;
  }
  return 0;
}
