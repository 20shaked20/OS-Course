/**
 * @file myFile.h
 * @author lior nagar and shaked levi
 * @brief
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXBLOCK 512
#define INODES 10

struct superblock
{
    int num_inodes;
    int num_blocks;
    int size_blocks;
};

struct inode
{
    int size;
    int first_block;
    char name[8];
};

struct disk_block
{
    int next_block_num;
    char data[MAXBLOCK];
};

/**
 * @brief this method mounts a file system into play
 * 
 * @param source 
 * @param target 
 * @param filesystemtype 
 * @param mountflags 
 * @param data 
 * @return int 
 */
int mymount(const char *source, const char *target,
            const char *filesystemtype, unsigned long mountflags, const void *data);

/**
 * @brief this method gets a pathname (filename) and opens it according to flags:
 * w - write
 * r - read
 * a - append 
 * etc.. 
 * @param pathname 
 * @param flags 
 * @return int 
 */
int myopen(const char *pathname, int flags);

/**
 * @brief this method gets a file descriptor (aka open file) and closes it.
 * 
 * @param myfd 
 * @return int 
 */
int myclose(int myfd);

/**
 * @brief this method gets a file descriptor, a buffer, and reads from the file.
 * 
 * @param myfd 
 * @param buf 
 * @param count 
 * @return ssize_t 
 */
ssize_t myread(int myfd, void *buf, size_t count);

/**
 * @brief this method gets a file descripotr and writes to it using the buffer.
 * 
 * @param myfd 
 * @param buf 
 * @param count 
 * @return ssize_t 
 */
ssize_t mywrite(int myfd, const void *buf, size_t count);

/**
 * @brief this method moves the currnet ?
 * 
 * @param myfd 
 * @param offset 
 * @param whence 
 * @return off_t 
 */
off_t mylseek(int myfd, off_t offset, int whence);