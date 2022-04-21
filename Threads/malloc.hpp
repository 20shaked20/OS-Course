
/**
 * @file malloc.hpp
 * @author lior nagar and shaked levi (you@domain.com)
 * @brief Header for for the malloc class
 * @version 0.1
 * @date 2022-04-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>

/**
 * @brief this method is a self implement of malloc like used in c library stdlib.
 * 
 * @param size size of block the user wishes to allocate
 * @return void* 
 */
void* my_malloc(size_t size);

/**
 * @brief this method is a self implement of calloc,
 * its same as malloc just when we set the data inside each block to 0.
 * 
 * @param size 
 * @return void* 
 */
void* my_calloc(size_t size);

/**
 * @brief this method is a self implemnt of free like used in c library stdlib.
 * 
 * @param ptr the pointer to free the memory block from.
 */
void my_free(void* ptr);