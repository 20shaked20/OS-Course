/**
 * @file myFile.c
 * @author lior nagar and shaked levi
 * @brief
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "myFile.h"
#include "utils.h"

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;

void mymkfs()
{
    sb.num_inodes = INODES;
    sb.num_blocks = 100;
    sb.size_blocks = sizeof(struct disk_block);

    int i = 0;
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for (i = 0; i < sb.num_inodes; i++)
    {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "emptyfi");
    }

    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
    for (i = 0; i < sb.num_blocks; i++)
    {
        dbs[i].next_block_num = -1;
    }
}
int mymount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data)
{
    /*TODO: fit MOUNT to assignment requirements*/
    FILE *file;
    file = fopen("fs_data", "r");

    /*superblock*/
    fread(&sb, sizeof(struct superblock), 1, file);

    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
    /*indoes*/
    fread(inodes, sizeof(struct inode), sb.num_inodes, file);
    fread(dbs, sizeof(struct disk_block), sb.num_blocks, file);

    fclose(file);
}

int myopen(const char *pathname, int flags)
{
    /*TODO: handle flags*/

    int in = find_empty_inode(sb, &inodes);
    int block = find_empty_disk_block(sb, &dbs);

    inodes[in].first_block = block;
    dbs[block].next_block_num = -2;

    strcpy(inodes[in].name, pathname);

    return in;
}

/*TODO: implement*/
int myclose(int myfd) {}
ssize_t myread(int myfd, void *buf, size_t count);

ssize_t mywrite(int myfd, const void *buff, size_t count)
{

    /*convert to char*/
    char *data = *((char *)buff);

    /*which block*/
    int relative_block = count / MAXBLOCK;

    /*find block number*/
    int bn = get_block_num(myfd, relative_block, &dbs, &inodes);

    /*offset in block*/
    int offset = count % MAXBLOCK;

    /*write data*/
    dbs[bn].data[offset] = (*data);
}
