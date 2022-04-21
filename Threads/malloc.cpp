/**
 * @file malloc.cpp
 * @author lior nagar and shaked levi (you@domain.com)
 * @brief implemenation of the class malloc.
 * @version 0.1
 * @date 2022-04-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <unistd.h> /*for sbrk */
#include <pthread.h> /* for thread safe */

#define align4(x) (((((x)-1) >> 2) << 2) + 4) /* used in calloc memory allocation for fixed location */

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; /*LOCK*/

typedef struct free_mem {
    size_t size;
    struct free_mem* next;
} free_mem;

static free_mem free_mem_head = { 0, 0 };
static const size_t overhead = sizeof(size_t);
static const size_t align_to = 16;

void* my_malloc(size_t size) {
    
    pthread_mutex_lock(&mutex1); /*LOCK*/

    size = (size + sizeof(size_t) + (align_to - 1)) & ~ (align_to - 1);
    free_mem* block = free_mem_head.next;
    free_mem** head = &(free_mem_head.next);
    while (block != 0) {
        if (block->size >= size) {
            *head = block->next;

            pthread_mutex_unlock(&mutex1); /*UNLOCK*/
            
            return ((char*)block) + sizeof(size_t);
        }
        head = &(block->next);
        block = block->next;
    }

    block = (free_mem*)sbrk(size);
    block->size = size;

    pthread_mutex_unlock(&mutex1); /*UNLOCK*/

    return ((char*)block) + sizeof(size_t);
}

void* my_calloc(size_t number, size_t size){

    pthread_mutex_lock(&mutex1); /*LOCK*/

    size_t *calloc;
    size_t s, i;

    calloc = (size_t*)my_malloc(number * size);

    if (calloc){
        s = align4(number * size) << 2;
        for (i = 0; i < s; i++){
            calloc[i] = 0;
        }
    }
    
    pthread_mutex_unlock(&mutex1); /*UNLOCK*/

    return calloc;
}

void my_free(void* ptr) {

    pthread_mutex_lock(&mutex1); /*LOCK*/

    free_mem* block = (free_mem*)(((char*)ptr) - sizeof(size_t));
    block->next = free_mem_head.next;
    free_mem_head.next = block;

    pthread_mutex_unlock(&mutex1); /*UNLOCK*/

}

