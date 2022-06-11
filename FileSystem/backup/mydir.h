/**
 * @file mydir.h
 * @author shaked levi & lior nagar
 * @brief
 * @version 0.1
 * @date 2022-06-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#define MAXSIZE 256

/*
 * Substitute for real dirent structure.  Note that `d_name' field is a
 * true character array although we have it copied in the implementation
 * dependent data.  We could save some memory if we had declared `d_name'
 * as a pointer refering the name within implementation dependent data.
 * We have not done that since some code may rely on sizeof(d_name) to be
 * something other than four.  Besides, directory entries are typically so
 * small that it takes virtually no time to copy them from place to place.
 */
typedef struct mydirent
{
    char d_name[MAXSIZE];
};

/* DIR substitute structure containing directory name.  The name is
 * essential for the operation of ``rewinndir'' function. */
typedef struct DIR
{
    char *dirname;     /* directory being scanned */
    struct mydirent current;    /* current entry */
    int dirent_filled; /* is current un-processed? */
} myDIR;


/*
 * Implement dirent interface as static functions so that the user does not
 * need to change his project in any way to use dirent function.  With this
 * it is sufficient to include this very header from source modules using
 * dirent functions and the functions will be pulled in automatically.
 */
static myDIR *myopendir(const char *name);
static struct mydirent *myreaddir(myDIR *dirp);
static int myclosedir(myDIR *dirp);
