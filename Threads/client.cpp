/**
 * @file client.cpp
 * @author lior nagar and shaked levi (you@domain.com)
 * @brief Implemnation of a simple client which will be used to connect to the server and send commands.
 * @version 0.1
 * @date 2022-04-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "stack.hpp"

#define PORT "3490" // the port client will be connecting to 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
void StackQueries(int sockfd){
	
	int can_recv;
	char buff[MAXSIZE];
	
	while(TRUE){

		bzero(buff,sizeof(buff));

		if(recv(sockfd, &buff,sizeof(buff),0) == -1){
			perror("client didnt recv...\n");
			exit(1);
		}else{
			printf("OUTPUT: ");
			printf("%s\n",buff);
		}
		
		bzero(buff,sizeof(buff));

		printf("Enter your request: ");
		fgets(buff,MAXSIZE,stdin);

		if(send(sockfd, buff, sizeof(buff),0) == -1){
			perror("write");
			exit(1);
		}

	}
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[MAXSIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure

	StackQueries(sockfd);


	close(sockfd);

	return 0;
}
