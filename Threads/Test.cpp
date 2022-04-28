/**
 * @file Test.cpp
 * @author shaked levi and lior nagar
 * @brief This is a tests file for the Threads assignment.
 * @version 0.1
 * @date 2022-04-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "stack.hpp"
#include "malloc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <assert.h>

/* Colors */
#define GREEN "\033[0;32m"
#define PURPLE "\033[0;35m"
#define WHITE "\033[0m"

/**
 * @brief a demo simulation for the client server.
 * 
 */
void run_demo(){

    const char *run_server = "gnome-terminal --command=\"./server\" ";
    system(run_server);
    sleep(2);

    const char *run_client_test1 = "gnome-terminal --command=\"./client_test1 127.0.0.1\" ";
    system(run_client_test1); /*client one */

    const char *run_client_test2 = "gnome-terminal --command=\"./client_test2 127.0.0.1\" ";
    system(run_client_test2); /*client two */

    const char *run_client_test3 = "gnome-terminal --command=\"./client_test3 127.0.0.1\" ";
    system(run_client_test3); /*client three */
}

/**
 * @brief some tests for checking the stack operations 
 * 
 * @return int 
 */
int main(){

    struct Stack *stacki = Stack();
    int ans = 1;
    
    char* s1 = (char*)malloc(sizeof(MAXSIZE));
    strcpy(s1,"Shaked\0");
    Push(stacki,s1);
    if(strcmp(s1,TOP(stacki)) != 0){ans = 0;}
    assert(ans); /*will be true*/

    char* s2 = (char*)malloc(sizeof(MAXSIZE));
    strcpy(s2,"Lior\0");
    Push(stacki,s2);
    if(strcmp(s2,TOP(stacki)) != 0){ans = 0;}; /* should be Lior */
    assert(ans);

    POP(stacki); /*pops lior */
    if(strcmp(s1,TOP(stacki)) != 0){ans = 0;} /*is equal to shaked*/
    assert(ans);

    
    printf(GREEN);
    printf("%s","Test complete... checking server client\n");
    printf(WHITE);

    free(s1);
    free(s2);

    run_demo();
    
    return 0;
}