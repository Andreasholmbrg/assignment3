#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080

void func(int sockFD)
{
  char buff[80];
  int n;

  for (;;)
  {
    bzero(buff, 80);

    read(sockFD, buff, sizeof(buff));

    printf("[x] From client: %s\t To client: ", buff);
    bzero(buff, 80);
    n = 0;

    while ((buff[n++] = getchar()) != '\n');

    write(sockFD, buff, sizeof(buff));

    if (strncmp("exit", buff, 4) == 0)
    {
      printf("[x] Server exit\n");
      break;
    }
  }
}

int main()
{
  int sockFD, connFD, len;
  struct sockaddr_in servaddr, cli;

  sockFD = socket(AF_INET, SOCK_STREAM, 0);
  if (sockFD == -1)
  {
  printf("[x] Socket creation failed\n");
  exit(0);
  }
  else
    printf("[x] Socket successfully created\n");
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if ((bind(sockFD, (struct sockaddr *) &servaddr, sizeof(servaddr))) != 0)
  {
    printf("[x] Socket bind failed\n");
    exit(0);
  }
  else
    printf("[x] Socket successfully binded\n");

  if ((listen(sockFD, 5)) != 0)
  {
    printf("[x] Listen failed\n");
    exit(0);
  }
  else
    printf("[x] Server listening\n");
  len = sizeof(cli);

  connFD = accept(sockFD, (struct sockaddr *) &cli, &len);
  if (connFD < 0)
  {
    printf("[x] Server accept failed\n");
    exit(0);
  }
  else
    printf("[x] Server accepted the client\n");

  func(connFD);

  close(sockFD);
}
