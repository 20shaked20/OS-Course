/**
 * @file utils.h
 * @author lior nagar and shaked levi
 * @brief
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "myFile.h"

/**
 * @brief Helper method to find the first empty inode
 * 
 * @param sb super block struct
 * @param inodes inode struct
 * @return int empty inode id
 */
int find_empty_inode(struct superblock sb, struct inode *inodes);

/**
 * @brief Helper method to find the first empty disk block
 * 
 * @param sb super block struct
 * @param dbs disk block struct
 * @return int empty disk block id
 */
int find_empty_disk_block(struct superblock sb, struct disk_block *dbs);

/**
 * @brief Get a desired the block num according to the offset
 * 
 * @param file file descriptor
 * @param offset how much to procced
 * @param dbs disk block struct
 * @param inodes inode struct
 * @return int desired id of disk block
 */
int get_block_num(int file, int offset, struct disk_block *dbs, struct inode *inodes);

/**
 * @brief this method needs to occur at the end of the test in order to sync the entire file system
 * (we should consider something else in the future)
 * @param sb 
 * @param dbs 
 * @param inodes 
 */
void sync_fs(struct superblock sb, struct disk_block *dbs, struct inode *inodes);

/**
 * @brief debug method to print the current file system
 * 
 * @param sb 
 * @param dbs 
 * @param inodes 
 */
void print_fs(struct superblock sb, struct disk_block *dbs, struct inode *inodes);