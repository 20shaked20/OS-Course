/**
 * @file myfs_test.c
 * @author shaked levi and lior nagar
 * @brief
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "myfs.h"

void main()
{

    /*first create a file system*/
    printf("PART A: creating a file system: \n");
    
    mymkfs();
    sync_fs();
    print_fs();

    printf("Creating file system success, now move to part B!...\n");
}