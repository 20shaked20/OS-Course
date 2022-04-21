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




/**
 * @brief a demo simulation for the client server.
 * 
 */
void run_demo(){

    const char *run_server = "gnome-terminal --command=\"./server\" ";
    system(run_server);
    sleep(2);

    const char *run_client = "gnome-terminal --command=\"./client 127.0.0.1\" ";
    system(run_client); /*client one */
    system(run_client); /*client two */
}

/**
 * @brief some tests for checking the stack operations 
 * 
 * @return int 
 */
int main(){

    struct Stack *stacki = Stack();
    
    char* s1 = (char*)malloc(sizeof(MAXSIZE));
    strcpy(s1,"Shaked\0");
    Push(stacki,s1);
    if(strcmp(s1,TOP(stacki)) != 0){perror("Not Equal");}

    char* s2 = (char*)malloc(sizeof(MAXSIZE));
    strcpy(s2,"Lior\0");
    Push(stacki,s2);
    if(strcmp(s2,TOP(stacki)) != 0){perror("Not Equal");}; /* should be Lior */

    POP(stacki); /*pops lior */
    if(strcmp(s1,TOP(stacki)) != 0){perror("Not Equal");} /*is equal to shaked*/

    char *s3 = (char*)malloc(sizeof(MAXSIZE));
    strcpy(s3,"ERROR: Stack is empty, nothing to TOP...\0");
    POP(stacki); /*empties stack*/
    if(strcmp(s3,TOP(stacki)) != 0){perror("Not Equal");} /*now stack is empty, should present s3.*/

    free(s1);
    free(s2);
    free(s3);
    run_demo();
    
    return 0;
}