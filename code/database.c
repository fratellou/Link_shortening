#include "database.h"

int main() {
  pthread_t thread;
  struct sockaddr_in server_address, client_address;
  pthread_mutex_init(&mutex, NULL);

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
    pthread_mutex_lock(&mutex);
    pthread_create(&thread, NULL, handle_client, (void *)&client_socket);
    pthread_join(thread, NULL);
    pthread_mutex_unlock(&mutex);
  }

  close(server_socket);
  pthread_mutex_destroy(&mutex);
  return 0;
}

void *handle_client(void *arg) {
  int client_socket = *((int *)arg);
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
    if (!strcmp(istr, " ")) break;
    strcpy(parsed_query[i], istr);
    istr = strtok(NULL, " ");
    i++;
  }

  request(db_file, parsed_query, req);
  size_t response_len = strlen(req);
  send(client_socket, req, response_len, 0);
  free(query);
  for (int j = 0; j < i; j++) {
    free(parsed_query[j]);
  }
  free(parsed_query);
  free(req);
  sleep(1);
  close(client_socket);

  printf("Connection completed\n");
  return NULL;
}

void request(char *db_file, char **query, char *req) {
  if (!strcmp(query[0], "HSET") || !strcmp(query[0], "HDEL") ||
      !strcmp(query[0], "HGET")) {
    hash(db_file, query, req);
  } else
    ERROR;
}
