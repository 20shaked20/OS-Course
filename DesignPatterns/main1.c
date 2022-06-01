/**
 * @file main1.c
 * @author lior nagar and shaked levi
 * @brief this is a redo for the queue and active object program..
 * @version 0.1
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 */

/****************************************/ /*PART 1*/ /****************************************/

/**
 * @brief thread safe QUEUE implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>  /*mallocs*/
#include <pthread.h> /*for threads*/
#include <ctype.h>   /*for strings*/

/*for server*/
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>

typedef struct Node
{
    void *next;
    void *value;
} node, *pnode;

typedef struct Queue
{
    pnode head;
    pnode tail;
    pthread_cond_t cond;
    pthread_mutex_t mutex;

} q, *ptr_q;

static ptr_q init_queue()
{

    /*init queue details*/
    ptr_q queue = (ptr_q)malloc(sizeof(q));
    queue->head = NULL;
    queue->tail = NULL;

    /*init cond & mutex*/
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    return queue;
}

static void free_queue(ptr_q queue)
{

    while (queue->head != NULL)
    {
        /*this will free all the elements in the queue.*/
        pnode to_delete = queue->head;
        queue->head = queue->head->next;
        free(to_delete);
    }
    /*destroy mutex and cont, also free the queue it self*/
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
    free(queue);
    queue = NULL;
}

static void enqueue(ptr_q queue, void *elem)
{
    /*Create new element*/
    pnode new_element = (pnode)malloc(sizeof(pnode));
    new_element->value = elem;
    new_element->next = NULL;

    if (queue->head == NULL)
    {
        /*in case head i empty, we will simply state the new element to be head & tail.*/
        queue->head = new_element;
        queue->tail = new_element;
    }
    else
    {
        /*in case queue i not empty, we will head the element to be last*/
        queue->tail->next = new_element;
        queue->tail = new_element;
    }
}

static void *dequeue(ptr_q queue)
{
    /*simly check if the queue is empty*/
    if (queue->head == NULL)
    {
        printf("Error: Queue is empty\n");
        return NULL;
    }
    /*fetch the node we want to get and then return its value*/
    /*CHECK FREE*/
    pnode return_node = queue->head;
    queue->head = queue->head->next;
    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }
    return return_node->value;
}

/**
 * @brief Queue cond implementation
 *
 */

struct CondQueue
{
    ptr_q queue;
    void *value;
};

pthread_mutex_t mutex_init = PTHREAD_MUTEX_INITIALIZER;

void *enQ(void *elem)
{
    /*init a cond queue*/
    struct CondQueue *cond_queue = (struct CondQueue *)elem;
    ptr_q queue = cond_queue->queue; /*the existense queue*/
    void *val = cond_queue->value;

    /*lock*/
    pthread_mutex_lock(&queue->mutex);

    enqueue(val, queue);
    printf("enqueue(cond) -> %d \n", *(int *)val); /*prints data for debuggin*/

    /*unlock*/
    pthread_mutex_unlock(&queue->mutex);

    /*condition variable*/
    pthread_cond_broadcast(&queue->cond);
}

char *deQ(void *elem)
{
    /*init a cond queue*/
    struct CondQueue *cond_queue = (struct CondQueue *)elem;
    ptr_q queue = cond_queue->queue; /*the existense queue*/
    void *val = cond_queue->value;

    /*lock*/
    pthread_mutex_lock(&queue->mutex);
    void *q_node;

    /*Wait condition*/
    while ((q_node = dequeue(queue)) == NULL)
    {
        printf("dequeue(cond) -> queue is waiting... \n");
        /*condi*/
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    printf("dequeue(cond) -> %d\n", *(int *)q_node); /*debug*/
    val = q_node;

    /*unlock*/
    pthread_mutex_unlock(&queue->mutex);
    return val;
}

ptr_q createQ()
{
    /*lock*/
    pthread_mutex_lock(&mutex_init);

    ptr_q q = init_queue();
    printf("createQ(cond)... \n");

    /*unlock*/
    pthread_mutex_unlock(&mutex_init);

    return q;
}
void destroyQ(ptr_q queue)
{
    /*lock*/
    pthread_mutex_lock(&mutex_init);

    printf("destroyQ(cond)... \n");
    free_queue(queue);

    /*unlock*/
    pthread_mutex_unlock(&mutex_init);
}

/****************************************/ /*PART 1 END*/ /****************************************/

/****************************************/ /*PART 2*/ /****************************************/

/**
 * @brief in this part we will create active object.
 *
 */

typedef struct ActiveObject
{

    /**
     * @brief
     * @param queue -> the queue we operate on.
     * @param before -> the method to be called at first on the object in the queue.
     * @param after -> the method to be called after we finish 'before' on the object in the queue.
     */
    ptr_q queue;
    void* (*before)(void *);
    void* (*after)(void *);

} active_object, *ptr_active_object;

/*check what to return/ to do with the ao*/
ptr_active_object newAO(ptr_q queue, void *before(void *), void *after(void *))
{
    /*init the active object*/
    ptr_active_object AO = (ptr_active_object)malloc(sizeof(ptr_active_object));
    AO->queue = queue;
    AO->before = before;
    AO->after = after;

    return AO;
}

void *destroyAO(ptr_active_object AO)
{
    pthread_cancel(AO->queue);
    free(AO->before);
    free(AO->after);
    free(AO);
}

/****************************************/ /*PART 2 END*/ /****************************************/

/****************************************/ /*PART 3*/ /****************************************/

/**
 * @brief in this part we will:
 * 1. create active object that will read from a socket using the server from ex3.
 * 2. will insert the data from (1) into the queue? or a list? idk.
 * 3. will create cesaer method that our active object (1) will do on each input that he get.
 * (big letters stay big letters HAL -> IBM, check on asqii)
 * 4. create a second active object that its func will be to lower case letters.
 * 5. create a third and last active object that its func will be the returned string after caeser/lower case.
 *
 * the pipeline will be created in this manner ->
 * AO_1 -> before = caeser, after = enqeue to AO_2 queue.
 * AO_2 -> before = to_lower_to_upper, after = enqueue to AO_3 queue.
 * AO_3 -> before = dequeue, after = return the str. /or opposite?/
 */

struct Pipeline
{
    char *elem;
    ptr_q queue;
    /*check if pointer needed or not*/
    ptr_active_object a_o1;
    ptr_active_object a_o2;
    ptr_active_object a_o3;
};

#define PORT "3490" /* the port users will be connecting to*/
#define BACKLOG 10  /* how many pending connections queue will hold*/
#define TRUE 1
#define FALSE 0
#define MAXSIZE 1024

struct Pipeline *our_pipe; /*is global so we can acess it in the handle client method*/

pthread_mutex_t server_mutex = PTHREAD_MUTEX_INITIALIZER;

void *caeser(char *str)
{
    int key = 1; /*the heset*/

    char *tmp = (char *)str;
    int i;
    for (i = 0; i < strlen(tmp); i++)
    {
        if (tmp[i] >= 'a' && tmp[i] <= 'z')
        {   
            if(tmp[i] == 'z')
            {
                tmp[i] = 'a';
                continue;
            }
            tmp[i] = tmp[i] + key;
            if (tmp[i] > 'z')
            {
                tmp[i] = tmp[i] - 'a';
            }
        }
        else if (tmp[i] >= 'A' && tmp[i] <= 'Z')
        {   
            if(tmp[i] == 'Z')
            {
                tmp[i] = 'A';
                continue;
            }
            tmp[i] = tmp[i] + key;
            if (tmp[i] > 'Z')
            {
                tmp[i] = tmp[i] - 'A';
            }
        }
    }
    our_pipe->elem = tmp;
    return NULL;
}

void *to_lower_to_upper(char *str)
{
    char *tmp = (char *)str;
    int i;
    for (i = 0; i < strlen(tmp); i++)
    {
        if (tmp[i] >= 'a' && tmp[i] <= 'z')
        {
            tmp[i] =toupper(tmp[i]);
        }
        else if (tmp[i] >= 'A' && tmp[i] <= 'Z')
        {
            tmp[i] = tolower(tmp[i]);
        }
    }
    /*keep the changed str*/
    our_pipe->elem = tmp;
    return NULL;
}

void *return_str(int client_fd)
{
    printf("THE ANSWER IS : %s\n", our_pipe->elem);

    if (send(client_fd, our_pipe->elem, MAXSIZE, 0) == -1)
    {
        perror("ERROR: sending pipelined message failed...\n");
    }
}

/**
 * @brief The server implementation.
 *
 */

void sigchld_handler(int s)
{
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void *handle_client(void *cli_sock)
{

    int new_fd = *((int *)cli_sock);

    pthread_mutex_lock(&server_mutex);

    send(new_fd, "Welcome client...", 20, 0);

    char cli_msg[MAXSIZE];
    int can_recv;

    while (TRUE)
    {

        bzero(cli_msg, MAXSIZE);

        /* read the message from client and copy it in buffer */
        if ((can_recv = recv(new_fd, cli_msg, MAXSIZE, 0)) == -1)
        {
            printf("ERROR: failed to receive data from client...\n");
            exit(1);
        }
        cli_msg[MAXSIZE - 1] = '\0';

        /* Prints the data the client requested */
        printf("Client Requested to: %s", cli_msg);
        cli_msg[strcspn(cli_msg, "\n")] = 0; /* removes '\n' */

        pthread_t t1;
        ptr_active_object ao1 = (ptr_active_object)our_pipe->a_o1;
        pthread_create(&t1, NULL, ao1->before, (void *)cli_msg);
        pthread_join(t1, NULL);
        printf("DEBUG: after ao1 %s , %d\n", our_pipe->elem, new_fd);

        pthread_t t2;
        ptr_active_object ao2 = ( ptr_active_object )our_pipe->a_o2;
        pthread_create(&t2, NULL, ao2->before, (void *)cli_msg);
        pthread_join(t2, NULL);
        printf("DEBUG: after ao2 %s , %d\n", our_pipe->elem, new_fd);

        pthread_t t3;
        ptr_active_object ao3 = (ptr_active_object)our_pipe->a_o3;
        pthread_create(&t3,NULL, ao3->before, (void *)new_fd);
        pthread_join(t3,NULL);

        bzero(cli_msg, MAXSIZE);

        pthread_mutex_unlock(&server_mutex);
    }
}

int main(void)
{

    /*active object init with pipelines*/
    our_pipe = (struct Pipeline *)malloc(sizeof(struct Pipeline));
    ptr_q q = createQ();
    our_pipe->elem = NULL;
    our_pipe->queue = q;

    /*init first AO */
    ptr_active_object p_ao1 = (ptr_active_object)malloc(sizeof(active_object));
    p_ao1->before = &caeser;
    our_pipe->a_o1 = p_ao1;

    /*init second AO */
    ptr_active_object p_ao2 = (ptr_active_object)malloc(sizeof(active_object));
    p_ao2->before = &to_lower_to_upper;
    our_pipe->a_o2 = p_ao2;

    /*init third AO */
    ptr_active_object p_ao3 = (ptr_active_object)malloc(sizeof(active_object));
    p_ao3->before = &return_str;
    our_pipe->a_o3 = p_ao3;

    /*server data init*/
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        /*Reset port */
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        /*Bind check*/
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    /*Listen check */
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("server failed to listen.. \n");
        exit(1);
    }
    printf("server: waiting for connections...\n");

    /*Init threadpool */
    pthread_t thread_pool[BACKLOG];
    int i = 0; // used for thread count.

    while (1)
    { // main accept() loop
        sin_size = sizeof their_addr;
        
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("failed to accpet client...\n");
            continue;
        }
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);

        printf("server: got connection from %s\n", s);

        /*Creates a new thread*/
        int thread_check = pthread_create(&thread_pool[i++], NULL, handle_client, &new_fd);
        if (thread_check != 0)
        {
            printf("Failed to create thread... \n");
        }

        /*join all threads.*/
        if (i >= BACKLOG)
        {
            i = 0;
            while (i < BACKLOG)
            {
                pthread_join(thread_pool[i++], NULL);
            }
            i = 0;
        }
    }

    return 0;
}
/// REMMBER TO INCLUDE -pthread!!@#@!