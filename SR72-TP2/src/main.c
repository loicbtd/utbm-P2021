#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

const char *SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 8080;
const int BUFFER_SIZE = 1024;

// Créer un serveur
void server()
{
  printf("[SERVER]\n");

  int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (serverSocket == -1)
  {
    perror("ERROR: cannot create socket");
    exit(EXIT_FAILURE);
  }
  printf("NOTICE: successfully created socket\n");

  if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
  {
    perror("ERROR: setsockopt(SO_REUSEADDR) failed");
    exit(EXIT_FAILURE);
  }
  printf("NOTICE: successfully set socket options\n");

  struct sockaddr_in serverSocketAddress;
  socklen_t serverSocketAddressLen = sizeof(serverSocketAddress);
  memset(&serverSocketAddress, 0, serverSocketAddressLen);
  serverSocketAddress.sin_family = AF_INET;
  serverSocketAddress.sin_addr.s_addr = INADDR_ANY;
  serverSocketAddress.sin_port = htons(SERVER_PORT);

  struct sockaddr_in clientSocketAddress;
  socklen_t clientSocketAddressLen = sizeof(clientSocketAddress);
  memset(&clientSocketAddress, 0, clientSocketAddressLen);

  if (bind(serverSocket, (struct sockaddr *)&serverSocketAddress, serverSocketAddressLen) == -1)
  {
    perror("ERROR: cannot bind socket");
    exit(EXIT_FAILURE);
  }
  printf("NOTICE: successfully binded socket\n");

  printf("\n\n");

  char buffer[BUFFER_SIZE];
  int readBytesCount = recvfrom(serverSocket, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&clientSocketAddress, &clientSocketAddressLen);
  buffer[readBytesCount] = '\0';
  printf("Client : %s\n", buffer);

  char *message = "Hello from server";
  sendto(serverSocket, (char *)message, strlen(message), MSG_CONFIRM, (struct sockaddr *)&clientSocketAddress, clientSocketAddressLen);
}

// Créer un client
void client()
{
  printf("[CLIENT]\n");

  int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (serverSocket == -1)
  {
    perror("ERROR: cannot create socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serverSocketAddress;
  socklen_t clientSocketAddressLen = sizeof(serverSocketAddress);
  memset(&serverSocketAddress, 0, clientSocketAddressLen);
  serverSocketAddress.sin_family = AF_INET;
  serverSocketAddress.sin_addr.s_addr = INADDR_ANY;
  serverSocketAddress.sin_port = htons(SERVER_PORT);

  // connect to server
  if (connect(serverSocket, (struct sockaddr *)&serverSocketAddress, clientSocketAddressLen) < 0)
  {
    perror("ERROR: connection failed \n");
    exit(EXIT_FAILURE);
  }

  printf("\n\n");

  char buffer[BUFFER_SIZE];
  char *message = "Hello from client";
  sendto(serverSocket, (char *)message, strlen(message), MSG_CONFIRM, (struct sockaddr *)&serverSocketAddress, clientSocketAddressLen);

  int readBytesCount = recvfrom(serverSocket, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&serverSocketAddress, &clientSocketAddressLen);
  buffer[readBytesCount] = '\0';
  printf("Server : %s\n", buffer);

  close(serverSocket);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("ERROR: bad argument count\n");
    return 1;
  }

  char *programName = argv[1];

  if (strcmp(programName, "server") == 0)
    server();
  if (strcmp(programName, "client") == 0)
    client();
}