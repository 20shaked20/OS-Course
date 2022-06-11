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
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <errno.h>

#define MAXBLOCK 512
#define INODES 10 /*#define MAXFILES 10000*/
#define MAXNAME 256

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
    char name[MAXNAME];
};

struct disk_block
{
    int next_block_num;
    char data[MAXBLOCK];
};


/*init*/
void mymkfs();

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
ssize_t mywrite(int myfd, const void *buff, size_t count);

/**
 * @brief this method moves the currnet ?
 *
 * @param myfd
 * @param offset
 * @param whence
 * @return off_t
 */
off_t mylseek(int myfd, off_t offset, int whence);


/*MY DIRENT METHODS*/

typedef struct mydirent
{
    char d_name[MAXNAME];
}mydirent;

typedef struct DIR
{
    char *dirname;     /* directory being scanned */
    struct mydirent current;    /* current entry */
    int dirent_filled; /* is current un-processed? */
} myDIR;

static myDIR *myopendir(const char *name);
static struct mydirent *myreaddir(myDIR *dirp);
static int myclosedir(myDIR *dirp);

/*HELPER METHODS*/

/**
 * @brief Helper method to find the first empty inode
 *
 * @return int empty inode id
 */
int find_empty_inode();

/**
 * @brief Helper method to find the first empty disk block
 *
 * @return int empty disk block id
 */
int find_empty_disk_block();

/**
 * @brief Get a desired the block num according to the offset
 *
 * @param file file descriptor
 * @param offset how much to procced
 * @return int desired id of disk block
 */
int get_block_num(int file, int offset);

/**
 * @brief this method needs to occur at the end of the test in order to sync the entire file system
 * (we should consider something else in the future)
 */
void sync_fs();

/**
 * @brief debug method to print the current file system
 *
 */
void print_fs();

/**
 * @brief Set the file size.
 * this method is invoked inside the myopen method, to default create a file with size of 5000.
 * it can be dynmaclly changed by calling it again.
 * 
 * @param fd 
 * @param size 
 */
void set_filesize(int fd, int size);