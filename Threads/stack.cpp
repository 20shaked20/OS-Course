/**
 * @file stack.cpp
 * @author lior nagar and shaked levi (you@domain.com)
 * @brief Implemantion of data structure 'stack' with mutex to support -thread safe- programming.
 * @version 0.1
 * @date 2022-04-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "malloc.hpp"
#include "stack.hpp"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /*LOCK*/

typedef struct StackNode {

    char *data;
    struct StackNode *next;

} StackNode;

struct Stack {

    size_t size;
    StackNode *top;

};

struct Stack *Stack(){

    struct Stack *stack = (struct Stack*)my_malloc(sizeof *stack);
	if(stack == NULL){
        printf("ERROR: in memory allocating for stack...\n");
        return NULL;
    }
	
	stack->size = 0;
	stack->top = NULL;
	
	return stack;

}

void Push(struct Stack *stack,char *text){

    pthread_mutex_lock(&mutex); /*LOCK*/

    StackNode *new_node = (StackNode*)my_malloc(sizeof(*new_node));
    if(new_node == NULL){
        printf("ERROR: in memory allocating for new node...\n");
        exit(1);
    }

    new_node->data = (char*)my_malloc(sizeof(MAXSIZE));
    if(new_node->data == NULL){
        printf("ERROR: in memory allocating for new data...\n");
        exit(1);
    }

    strcpy((new_node->data),text);
    new_node->next = stack->top;
    stack->top = new_node;

    stack->size +=1;

    pthread_mutex_unlock(&mutex); /*UNLOCK*/
}

char* POP(struct Stack *stack){

    pthread_mutex_lock(&mutex); /*LOCK*/

    StackNode *prev;

    char *data = (char*)my_malloc(sizeof(MAXSIZE));
    if(data == NULL){
        printf("ERROR: in memory allocating for data...\n");
        exit(1);
    }

    if(stack->top == NULL){
        printf("ERROR: Stack is empty, nothing to POP...\n");
    }else{
        prev = stack->top;
        strcpy(data,prev->data);
        stack->top = prev->next;

        my_free(prev->data);
        my_free(prev);
        prev = NULL;

        stack->size -=1;
    }
    
    pthread_mutex_unlock(&mutex); /*UNLOCK*/

    return data;
}

char* TOP(struct Stack *stack){

    pthread_mutex_lock(&mutex); /*LOCK*/
    if(stack->size != 0 ){

        pthread_mutex_unlock(&mutex); /*UNLOCK*/

        return stack->top->data;
    }

    pthread_mutex_unlock(&mutex); /*UNLOCK*/
    
    return "ERROR: Stack is empty, nothing to TOP...";
}

void empty_stack(struct Stack *stack){

    while(stack->size!=0){
        POP(stack);
    }
}

void free_stack(struct Stack *stack){
    empty_stack(stack);
    my_free(stack);
}

/*used for debug and testing*/

// int main(){

//     struct Stack *stacki = Stack();
//     char* tmp = "bla";
//     Push(stacki,tmp);
//     printf("%zu\n",stacki->size);
//     TOP(stacki);
//     char* tmp2 = "fuckuuuu";
//     Push(stacki,tmp2);
//     printf("%zu\n",stacki->size);
//     TOP(stacki);

//     char *tmp3 = (char*)malloc(sizeof(MAXSIZE));
//     tmp3 = POP(stacki);
//     printf("%zu\n",stacki->size);
//     printf("%s\n",TOP(stacki));
//     POP(stacki);
//     printf("%s\n",TOP(stacki));
//     printf("%s\n",TOP(stacki));
//     POP(stacki);
//     TOP(stacki);
//     printf("BLA\n");
//     my_free(tmp3);
//     free_stack(stacki);
//     return 0;
// }   