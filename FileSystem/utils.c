/**
 * @file utils.c
 * @author lior nagar and shaked levi
 * @brief
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "utils.h"

int find_empty_inode(struct superblock sb, struct inode *inodes)
{
    int i = 0;
    for (i = 0; i < sb.num_inodes; ++i)
    {
        if (inodes[i].first_block == -1)
        {
            return i;
        }
    }
    return -1;
}

int find_empty_disk_block(struct superblock sb, struct disk_block *dbs)
{
    int i = 0;
    for (i = 0; i < sb.num_blocks; ++i)
    {
        if (dbs[i].next_block_num == -1)
        {
            return i;
        }
    }
    return -1;
}

/*could be lseek*/
int get_block_num(int file, int offset, struct disk_block *dbs, struct inode *inodes)
{

    int togo = offset;
    int bn = inodes[file].first_block;

    while (togo > 0)
    {
        bn = dbs[bn].next_block_num;
        --togo;
    }
    return bn;
}

void sync_fs(struct superblock sb, struct disk_block *dbs, struct inode *inodes)
{
    FILE *file;
    file = fopen("fs_data", "w+");

    /*superblock*/
    fwrite(&sb, sizeof(struct superblock), 1, file);

    int i = 0;
    for (i = 0; i < sb.num_inodes; ++i)
    {
        fwrite(&inodes[i], sizeof(struct inode), 1, file);
    }
    for (i = 0; i < sb.num_blocks; ++i)
    {
        fwrite(&dbs[i], sizeof(struct disk_block), 1, file);
    }

    fclose(file);
}

void print_fs(struct superblock sb, struct disk_block *dbs, struct inode *inodes)
{

    printf("Superblock info\n");
    printf("\tnum inodes: %d\n", sb.num_inodes);
    printf("\tnum blocks: %d\n", sb.num_blocks);
    printf("\tsize blocks: %d\n", sb.size_blocks);

    printf("inodes: \n");
    int i = 0;
    for (i = 0; i < sb.num_inodes; ++i)
    {
        printf("\tsize: %d block: %d name: %s\n", inodes[i].size, inodes[i].first_block, inodes[i].name);
    }

    for (i = 0; i < sb.num_blocks; ++i)
    {
        printf("\tblock num: %d next block: %d\n", i, dbs[i].next_block_num);
    }
}