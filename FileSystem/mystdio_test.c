/**
 * @file myFile_test.c
 * @author lior nagar and shaked levi
 * @brief simple test class for myFILE.
 * @version 0.1
 * @date 2022-06-10
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "mystdio.h"
#define GREEN "\033[0;32m"
#define WHITE "\033[0m"
#define PURPLE "\033[0;35m"


/*we demonstrate the methods we implemented using this simple test*/

void main()
{
    printf(GREEN);

    printf("Mounting the file system: ...\n");
    sleep(1);
    mymount("fs_data", "def", "def", 0, "def"); /*must have at init*/


    myFILE *file1;
    file1 = myfopen("file1", "r+");

    myFILE *file2;
    file2 = myfopen("file2","r+");

    myFILE *file3;
    file3 = myfopen("file3","r");
    
    printf("Printing file system data after file creation:..\n");

    printf(WHITE);

    print_fs();

    /*string sizes*/
    size_t size;
    size_t nmemb;

    size_t write;
    size_t read;

    /*file1 tests*/
    void *ptr1 = "liorlior";
    size = 4;
    nmemb = 2;

    printf(GREEN);

    printf("Writing to file 1...\n");
    sleep(1);
    write = myfwrite(ptr1, size, nmemb, file1);
    
    printf(WHITE);

    printf("Reading from file 1 what we wrote: ");
    read = myfread(ptr1, size, nmemb, file1);

    void *ptr3 = "writng a third test";
    size = 5;
    nmemb = 4;

    printf(GREEN);

    printf("Writing to file 1...\n");
    sleep(1);
    write = myfwrite(ptr3, size, nmemb, file1);
    
    printf(WHITE);

    printf("Reading from file 1 what we wrote: ");
    read = myfread(ptr3, size, nmemb, file1);

    /*file2 tests*/
    void *ptr2 = "hello!, im just testing thee files!";
    size = 6;
    nmemb = 6;

    printf(GREEN);

    printf("Writing to file 2...\n");
    sleep(1);
    write = myfwrite(ptr2, size, nmemb, file1);
    
    printf(WHITE);

    printf("Reading from file 2 what we wrote: ");
    read = myfread(ptr2, size, nmemb, file1);

    /*file 3 edge cases testings*/
    void *ptr4 = "cant write to file3";
    size = 5;
    nmemb = 4;

    printf(GREEN);
    printf("Writing to file 3...\n");
    sleep(1);
    printf(PURPLE);

    write = myfwrite(ptr4, size, nmemb, file3);

    
    /*closing*/
    printf(GREEN);

    printf("Closing files...\n");
    sleep(1);
    myfclose(file1);
    myfclose(file2);

    sync_fs(); /*must have at finish*/

    printf("File system empty again: \n");

    printf(WHITE);
    
    print_fs();
}