/**
 * @file ThreadServer.cpp
 * @author lior nagar and shaked levi (you@domain.com)
 * @brief Implemnation of threaded server which support multiple client connection.
 * this server is using a stack to insert and display the data inside of it under lock supervision to ensure 
 * -safe thread- program.
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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h> // for threads

#include "stack.hpp"

#define PORT "3490"  /*the port users will be connecting to*/

#define BACKLOG 10	 /*how many pending connections queue will hold*/

struct Stack *stacki; /*global stack*/

void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// void *create_socket(void *args){

//     int new_fd = *((int*)args);
//     if (send(new_fd, "Welcome client!...\n", 17, 0) == -1){
// 		perror("sending failed...\n");
//     }
// 	close(new_fd);
// 	printf("client socket closed...\n");
// }

void *StackQueries(void *cli_sock){

	int new_fd = *((int*)cli_sock);
	
	send(new_fd, "Welcome client...", 20, 0);

	char cli_msg[MAXSIZE];
	int can_recv;

    while(TRUE) {

        bzero(cli_msg, MAXSIZE);
		
        /* read the message from client and copy it in buffer */
        if ((can_recv = recv(new_fd, cli_msg, MAXSIZE, 0)) == -1) {
			printf("ERROR: failed to receive data from client...\n");
			exit(1);
		}
		cli_msg[MAXSIZE-1] = '\0';

        /* Prints the data the client requested */
        printf("Client Requested to: %s",cli_msg);
		cli_msg[strcspn(cli_msg,"\n")] = 0; /* removes '\n' */
		int i = 0;
        /* in case we got Exit, return to main menu and stop recieving from the server, return to normal */
        if (strncmp("Push", cli_msg, 4) == 0) {
            memmove(cli_msg,cli_msg+5,sizeof(cli_msg)); /*removes the Push from the string */
			if (send(new_fd, "PUSHING..", 10, 0) == -1){
				perror("ERROR: sending failed...\n");
    		}
			Push(stacki,cli_msg);
        }else if(strncmp("POP",cli_msg,3) == 0){
			if (send(new_fd, "POPPING..", 10, 0) == -1){
				perror("ERROR: sending failed...\n");
    		}
			POP(stacki);
		}else if(strncmp("TOP",cli_msg,3) == 0){
			if (send(new_fd, TOP(stacki), MAXSIZE, 0) == -1){
				perror("ERROR: sending failed...\n");
    		}
		}else if(strncmp("Exit",cli_msg,4) == 0){
			printf("Server Exit...\n");
			close(new_fd);
		}else{
			printf("ERROR: bad command, enter again...\n");
		}
		bzero(cli_msg, MAXSIZE);
    }
}


int main(void)
{
	stacki = Stack(); /* init global stack */
	int sockfd, new_fd;  /* listen on sock_fd, new connection on new_fd */
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			printf("ERROR: failed find bind socket... \n");
			continue;
		}

		/*Reset port */
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			printf("ERROR: failed to apply setsockopt");
			exit(1);
		}

        /*Bind check*/
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			printf("ERROR: failed to bind server socket");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

    /*Listen check */
	if (listen(sockfd, BACKLOG) == -1) {
		printf("ERROR: server failed to listen.. \n");
		exit(1);
	}
	printf("server: waiting for connections...\n");

    /*Init threadpool */
    pthread_t thread_pool[BACKLOG];
    int i = 0; // used for thread count.
	int pids[BACKLOG]; /*the id of threads */
	
	while(TRUE) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			printf("ERROR: failed to accpet client...\n");
			// continue;
		}
		inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);

		printf("server: got connection from %s client ID: %d\n", s,i);
        /*Creates a new thread*/
        int tpid = pthread_create(&thread_pool[i++], NULL, StackQueries, &new_fd);
        if(tpid != 0){
            printf("Failed to create thread... \n");
        }

        // join all threads.
        if ( i >= BACKLOG){
            i = 0;
            while (i < BACKLOG){
                pthread_join(thread_pool[i++],NULL);
            }
            i = 0;
        }
	}
	free_stack(stacki);
	close(sockfd);

	return 0;
}
/// REMMBER TO INCLUDE -pthread!!@#@!