/**
 * @file myFile.c
 * @author shaked levi & lior nagar
 * @brief
 * @version 0.1
 * @date 2022-06-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "mystdio.h"

myFILE *myfopen(const char *pathname, const char *mode)
{
    size_t len = strlen(pathname);

    if (len > 0)
    {
        int fd;

        myFILE *fp;
        fp = (myFILE *)malloc(sizeof(myFILE));

        /*flags*/
        if (mode[0] == 'w')
        {
            fd = myopen(pathname, WRONLY);
            fp->flag = WRONLY;
        }
        else if (strcmp(mode,"r+") == 0)
        {   
            fd = myopen(pathname, RDWR);
            fp->flag = RDWR;
        }
        else if (mode[0] == 'r')
        {   
            fd = myopen(pathname, RDONLY);
            fp->flag = RDONLY;
        }
        else if (mode[0] == 'a')
        {
            fd = myopen(pathname, APPEND);
            fp->flag = APPEND;
        }
        else
        {
            printf("ERROR: you chose to open the file with unsupported mode.\n");
            return NULL;
        }

        if (fd < 0)
        {
            printf("ERROR: couldn't open file\n");
            return NULL;
        }
        fp->ptr = fd;
        fp->data = "";
        fp->size = 0;

        return fp;
    }
    printf("ERROR: invalid pathname\n");
    return NULL;
}

int myfclose(myFILE *stream)
{
    int check = myclose(stream->ptr);
    if (check == 0)
    {
        stream->data = NULL;
        stream->size = 0;
        free(stream);
        return 0; /*true*/
    }
    printf("ERROR: failed to close file\n");
    return -1; /*failed*/
}

size_t myfread(void *ptr, size_t size, size_t nmemb, myFILE *stream)
{
    /*calls the myread method*/
    if (stream->flag == RDONLY || stream->flag == RDWR)
    {
        myread(stream->ptr, ptr, nmemb * size);

        return (nmemb);
    }
}

size_t myfwrite(const void *ptr, size_t size, size_t nmemb, myFILE *stream)
{
    if (stream->flag == WRONLY || stream->flag == APPEND || stream->flag == RDWR)
    {
        void *tmp = ptr;
        int i;
        for (i = 0; i < nmemb * size; ++i)
        {
            mywrite(stream->ptr, tmp, i);
            tmp++;
        }
        return (nmemb);
    }
    else
    {
        printf("ERROR: file is not open for writing\n");
        return 0;
    }
}

int myfseek(myFILE *stream, long offset, int whence){return 0;}

int myfscanf(myFILE *stream, const char *format, ...){return 0;}

int myfprintf(myFILE *stream, const char *format, ...){return 0;}