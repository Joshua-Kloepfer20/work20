#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  mkfifo(WKP, 0644);
  printf("created WKP\n");
  int from_client = open(WKP, O_RDONLY);
  char buffer[BUFFER_SIZE];
  read(from_client, buffer, BUFFER_SIZE - 1);
  printf("Received message\n");
  *to_client = open(buffer, O_WRONLY);
  write(*to_client, "msg", 4);
  printf("wrote to client\n");
  read(from_client, buffer, BUFFER_SIZE - 1);
  printf("read reasponse\n");
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  mkfifo(ACK, 0644);
  *to_server = open(WKP, O_WRONLY);
  write(*to_server, ACK, 5);
  int from_server = open(ACK, O_RDONLY);
  char buffer[BUFFER_SIZE];
  read(from_server, buffer, BUFFER_SIZE - 1);
  int i;
  for (i = 0; i < strlen(buffer); i++) {
    buffer[i] = buffer[i] + 1;
  }
  write(*to_server, buffer, strlen(buffer));
  return from_server;
}
