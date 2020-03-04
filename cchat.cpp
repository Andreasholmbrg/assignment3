#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SA struct sockaddr
void func(int sockfd)
{
	char buff[80];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("[x] Enter string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("[x] From Server : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("[x] Client Exit\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("[x] Socket creation failed\n");
		exit(0);
	}
	else
		printf("[x] Socket successfully created\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("[x] Failed to connect to server\n");
		exit(0);
	}
	else
		printf("[x] Connected to the server\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}
