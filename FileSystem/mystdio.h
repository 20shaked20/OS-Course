/**
 * @file myFile.h
 * @author shaked levi and lior nagar
 * @brief
 * @version 0.1
 * @date 2022-06-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "myfs.h"

#define RDWR 1
#define APPEND 2
#define RDONLY 3
#define WRONLY 4

typedef struct myFILE
{
    char *data;
    int size;
    int ptr;
    int flag;

} myFILE;

/**
 * @brief this method calls the mymkfs method "myopen" and creates a new file.
 *
 * @param pathname
 * @param mode
 * @return myFILE*
 */
myFILE *myfopen(const char *pathname, const char *mode);

/**
 * @brief this method closes a file in the file system.
 *
 * @param stream This is the pointer to a FILE object that specifies an input stream.
 * @return int
 */
int myfclose(myFILE *stream);

/**
 * @brief
 *
 * @param ptr This is the pointer to a block of memory with a minimum size of size*nmemb bytes.
 * @param size This is the size in bytes of each element to be read.
 * @param nmemb This is the number of elements, each one with a size of size bytes.
 * @param stream This is the pointer to a FILE object that specifies an input stream.
 * @return size_t total of read elements
 */
size_t myfread(void *ptr, size_t size, size_t nmemb, myFILE *stream);

/**
 * @brief
 *
 * @param ptr This is the pointer to a block of memory with a minimum size of size*nmemb bytes.
 * @param size This is the size in bytes of each element to be read.
 * @param nmemb This is the number of elements, each one with a size of size bytes.
 * @param stream This is the pointer to a FILE object that specifies an input stream.
 * @return size_t total of written elements
 */
size_t myfwrite(const void *ptr, size_t size, size_t nmemb, myFILE *stream);

/* we didnt implement those */
int myfseek(myFILE *stream, long offset, int whence);

int myfscanf(myFILE *stream, const char *format, ...);

int myfprintf(myFILE *stream, const char *format, ...);