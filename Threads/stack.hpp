/**
 * @file stack.h
 * @author shaked levi and lior nagar (you@domain.com)
 * @brief This is the header file for the stack class
 * @version 0.1
 * @date 2022-04-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// #ifndef STACK_STACK_H
// #define STACK_STACK_H

#define MAXSIZE 1024
#define TRUE 1
#define FALSE 0

/**
 * @brief 
 * @param top -> a single text, using a single pointer.
 * @param size -> unsinged size of the stack
 * @param list -> list of texts, is created by double pointer array 
 */
// struct Stack {

//     char *top;
//     unsigned size;
//     char ** list;

// }stack;


/**
 * @brief Constructor for the stack, creates one empty with a fixed size of 'Size'
 * 
 * @param size 
 * @return struct Stack* 
 */
struct Stack *Stack();


/**
 * @brief insert a text into the stack in LIFO way
 * 
 * @param text pointer contains the text
 */
void Push(struct Stack *stack,char *text);

/**
 * @brief pops the top text in the stack.
 * 
 * @return char* returns the text
 */
char *POP(struct Stack *stack);


/**
 * @brief displays the stack top.
 * 
 */
char* TOP(struct Stack *stack);

/**
 * @brief this method frees the memory allocation for the stack.
 * 
 */
void free_stack(struct Stack *stack);

// #endif //STACK_STACK_H