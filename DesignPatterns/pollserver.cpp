/*
** pollserver.c -- a cheezy multiperson chat server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <pthread.h>

#include "reactor.hpp"

#define PORT "9034" // Port we're listening on
#define MAXSIZE 1024
#define TRUE 1
#define FALSE 0

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket(void)
{
    int listener; // Listening socket descriptor
    int yes = 1;  // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0)
    {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0)
        {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai); // All done with this

    // If we got here, it means we didn't get bound
    if (p == NULL)
    {
        return -1;
    }

    // Listen
    if (listen(listener, 10) == -1)
    {
        return -1;
    }

    return listener;
}

// Add a new file descriptor to the set
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size)
    {
        *fd_size *= 2; // Double it

        *pfds = (pollfd *)realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

    (*fd_count)++;
}

// Remove an index from the set
void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
    // Copy the one from the end over this one
    pfds[i] = pfds[*fd_count - 1];

    (*fd_count)--;
}
/* CODE WONT COMPILE UNLESS I PUT IT HERE */
void InstallHandler(Reactor *rec, void *func(void *), int *fd)
{

    for (size_t i = 0; i < rec->_fds.size(); ++i)
    {
        if (rec->_fds.at(i)->fd == *fd)
        {
            /*case where we already have this fd,
             * we will simply change the handler.
             */

            rec->_fds.at(i)->func = &func;
        }
    }
    /*else: create a new fd handler and insert to vector*/
    struct fd_handlers *assign = (struct fd_handlers *)malloc(sizeof(struct fd_handlers));
    assign->fd = *fd;
    assign->func = &func;
    assign->temp = 0;
    pthread_create(&assign->temp,NULL,func,(void*)fd); /*creating the thread to handle the function*/
    rec->_fds.push_back(assign);
    
}

/**
 * @brief this is the thread function to handle each client
 * 
 * @param client_fd 
 * @return void* 
 */
void *run(void *client_fd)
{
    /*handle client socket*/
    int *p_new_fd = (int *)client_fd;
    int new_fd = *p_new_fd;
    printf("Started thread for fd %d\n", new_fd);
    /*welcome msg*/
    send(new_fd, "Welcome client :) ", 20, 0);

    char msg[MAXSIZE];

    while (TRUE)
    {
        /*recv data*/
        int nbytes = recv(new_fd, msg, sizeof(msg), 0);

        if (nbytes <= 0)
        {
            /* Got error or connection closed by client */
            if (nbytes == 0)
            {
                /* Connection closed */
                printf("PollServer: socket %d hung up\n", new_fd);
            }
            else
            {
                /*recv problem*/
                perror("ERROR: problem while recveing data");
            }
            // RemoveHandler()
            close(new_fd); // Bye!
        }
        else
        {
            /* We got some good data from a client */
            /*we simply pong back the message to the client.. just to demonstarte how it works*/
            send(new_fd, msg, sizeof(msg),0);
        }
    }
}

void *newReactor()
{
    Reactor *newReactor = new Reactor();
    return newReactor;
}

/* Main */
int main(void)
{
    int listener; /* Listening socket descriptor */

    int newfd;                          // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char buf[256]; // Buffer for client data

    char remoteIP[INET6_ADDRSTRLEN];

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    int fd_count = 0;
    int fd_size = 5;
    struct pollfd *pfds = (pollfd *)malloc(sizeof *pfds * fd_size);

    // Set up and get a listening socket
    listener = get_listener_socket();

    if (listener == -1)
    {
        fprintf(stderr, "ERROR: getting listening socket\n");
        exit(1);
    }

    // Add the listener to set
    pfds[0].fd = listener;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection

    fd_count = 1; // For the listener

    // Main loop
    while (TRUE)
    {
        int poll_count = poll(pfds, fd_count, -1);

        if (poll_count == -1)
        {
            perror("ERROR: poll");
            exit(1);
        }

        // Run through the existing connections looking for data to read
        for (int i = 0; i < fd_count; i++)
        {

            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN)
            { // We got one!!

                if (pfds[i].fd == listener)
                {
                    // If listener is ready to read, handle new connection

                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                                   (struct sockaddr *)&remoteaddr,
                                   &addrlen);

                    if (newfd == -1)
                    {
                        perror("ERROR: failed to accept");
                    }
                    else
                    {
                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);

                        printf("PollServer: new connection from %s on "
                               "socket %d\n",
                               inet_ntop(remoteaddr.ss_family,
                                         get_in_addr((struct sockaddr *)&remoteaddr),
                                         remoteIP, INET6_ADDRSTRLEN),
                               newfd);
                        Reactor *reactor = (Reactor *)newReactor();
                        InstallHandler(reactor, &run, &newfd);
                    }
                }
            } // END got ready-to-read from poll()
        }     // END looping through file descriptors
    }         // END for(;;)--and you thought it would never end!

    return 0;
}