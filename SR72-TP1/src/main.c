#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/un.h>
#include <netinet/in.h>

char *socket_path = "/tmp/localSocket";

// Créer une socket de type SOCK_DGRAM
int exercice1p1()
{
  int localSocket = socket(AF_INET, SOCK_DGRAM, 0);

  if (localSocket == -1)
  {
    perror("ERROR: cannot create UDP socket");
    exit(1);
  }

  printf("NOTICE: UDP socket %d has been created\n", localSocket);
  sleep(100);
  close(localSocket);
  return 0;
}

// Créer une socket et l'attacher à un port
int exercice1p2()
{
  struct sockaddr_in localSocketAddress;

  int localSocket = socket(AF_INET, SOCK_DGRAM, 0);

  if (localSocket == -1)
  {
    perror("ERROR: cannot create UDP socket");
    return (1);
  }

  printf("NOTICE: UDP socket %d has been created\n", localSocket);

  localSocketAddress.sin_family = AF_INET;
  localSocketAddress.sin_port = htons(7000);
  localSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(localSocket, (struct sockaddr *)&localSocketAddress, sizeof(localSocketAddress)) != 0)
  {
    perror("ERROR: cannot bind socket");
    close(localSocket);
    return (1);
  }

  printf("NOTICE: socket binded to port %d\n", ntohs(localSocketAddress.sin_port));
  sleep(200);
  close(localSocket);

  return 0;
}

// Créer un serveur
int server()
{
  struct sockaddr_un localSocketAddress;
  char buffer[100];
  int localConnection;
  int reader;

  int localSocket = socket(AF_UNIX, SOCK_STREAM, 0);

  if (localSocket == -1)
  {
    perror("ERROR: cannot create socket");
    return (-1);
  }

  memset(&localSocketAddress, 0, sizeof(localSocketAddress));
  localSocketAddress.sun_family = AF_UNIX;
  strncpy(localSocketAddress.sun_path, socket_path, sizeof(localSocketAddress.sun_path) - 1);
  unlink(socket_path);

  if (bind(localSocket, (struct sockaddr *)&localSocketAddress, sizeof(localSocketAddress)) == -1)
  {
    perror("ERROR: cannot bind socket");
    return (-1);
  }

  if (listen(localSocket, 5) == -1)
  {
    perror("ERROR: cannot listen socket");
    return (-1);
  }

  while (1)
  {
    if ((localConnection = accept(localSocket, NULL, NULL)) == -1)
    {
      perror("ERROR: cannot accept connection on socket");
      continue;
    }

    while ((reader = read(localConnection, buffer, sizeof(buffer))) > 0)
    {
      printf("Read %u bytes: %.*s\n", reader, reader, buffer);
    }
    if (reader == -1)
    {
      perror("ERROR: cannot read from connection");
      exit(-1);
    }
    else if (reader == 0)
    {
      printf("EOF\n");
      close(localConnection);
    }
  }

  return 0;
}

// Créer un client
int client()
{
  struct sockaddr_un localSocketAddress;
  char buffer[100];
  int readerCount;

  int localSocket = socket(AF_UNIX, SOCK_STREAM, 0);

  if (localSocket == -1)
  {
    perror("ERROR: cannot create socket");
    return -1;
  }

  memset(&localSocketAddress, 0, sizeof(localSocketAddress));
  localSocketAddress.sun_family = AF_UNIX;
  strncpy(localSocketAddress.sun_path, socket_path, sizeof(localSocketAddress.sun_path) - 1);

  if (connect(localSocket, (struct sockaddr *)&localSocketAddress, sizeof(localSocketAddress)) == -1)
  {
    perror("ERROR: cannot connect to socket");
    return -1;
  }

  while ((readerCount = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
  {
    if (write(localSocket, buffer, readerCount) != readerCount)
    {
      if (readerCount > 0)
      {
        fprintf(stderr, "ERROR: partial buffer write");
      }
      else
      {
        perror("ERROR: write error");
        return -1;
      }
    }
  }

  return 0;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("ERROR: bad argument count\n");
    return 1;
  }

  char *programName = argv[1];

  if (strcmp(programName, "exercice1p1") == 0)
    return exercice1p1();
  if (strcmp(programName, "exercice1p2") == 0)
    return exercice1p2();
  if (strcmp(programName, "server") == 0)
    return server();
  if (strcmp(programName, "client") == 0)
    return client();
}