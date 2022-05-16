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
 * @brief this is just a defenition of the struct, the main code resides in the process server.
 * the idea was to implenet it as an array because the data is straitforward and this way i can acess it by 
 * counting the the current place, and once i pop something i just decrease the size by one.
 * (is all stated in the ProcessServer.cpp)
 * @param txt -> a single text, using the maximu size it can get
 * @param next -> the next string in the stack
 */


struct Stack {

    struct Stack *next;
    char txt[MAXSIZE];

};

// #endif //STACK_STACK_H