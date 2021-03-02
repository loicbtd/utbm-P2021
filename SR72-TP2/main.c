#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void exercice1()
{
  int udp_s;

  if ((udp_s = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    perror("création socket udp");
    exit(1);
  }
  printf("socket udp %d créé\n", udp_s);
  sleep(100);
  close(udp_s);
}

int main()
{
  exercice1();
  return 0;
}