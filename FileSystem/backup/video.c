#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    char data[512];
};

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;

void create_fs()
{
    sb.num_inodes = 10;
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
void mount_fs()
{
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
void sync_fs()
{
    FILE *file;
    file = fopen("fs_data", "w+");

    /*superblock*/
    fwrite(&sb, sizeof(struct superblock), 1, file);

    int i = 0;
    for(i = 0; i< sb.num_inodes; ++i)
    {
        fwrite(&inodes[i],sizeof(struct inode), 1, file);
    }
     for(i = 0; i< sb.num_blocks; ++i)
    {
        fwrite(&dbs[i],sizeof(struct disk_block), 1, file);
    }

    fclose(file);
}

void print_fs()
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

int find_empty_inode()
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

int find_empty_disk_block()
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

/*return fd*/
int allocate_file(char name[8])
{

    int in = find_empty_inode();
    int block = find_empty_disk_block();

    inodes[in].first_block = block;
    dbs[block].next_block_num = -2;

    strcpy(inodes[in].name, name);

    return in;
}

void shorten_file(int bn)
{
    int nn = dbs[bn].next_block_num;
    if(nn >= 0)
    {
        shorten_file(nn);
    }
    dbs[bn].next_block_num = -1;
}

void set_filesize(int filenum, int size){
    int tmp = size + 512 -1;
    int num = tmp / 512;

    int bn = inodes[filenum].first_block;
    num--;
    //Increase file if needed
    while(num>0){
        /*next block number*/
        int next_num = dbs[bn].next_block_num;
        if (next_num == -2){
            /*case where there is no next block available so we will find empty one*/
            int empty = find_empty_disk_block();
            dbs[bn].next_block_num = empty;
            dbs[empty].next_block_num = -2;
        }
        bn = dbs[bn].next_block_num;
        num--;
    }

    // shorten file in needed
    shorten_file(bn);
    dbs[bn].next_block_num = -2; /*end*/
    
}

int get_block_num(int file, int offset){

    int togo = offset;
    int bn = inodes[file].first_block;

    while(togo > 0){
        bn = dbs[bn].next_block_num;
        --togo;
    }
    return bn;
}

void write_byte(int filenum, int pos, char*data){

    /*which block*/
    int relative_block = pos / 512;
    /*find block number*/
    int bn = get_block_num(filenum, relative_block);
    /*offset in block*/
    int offset = pos % 512;
    /*write data*/
    dbs[bn].data[offset] = (*data);
}

void main()
{

    /*first create a file system*/
    create_fs();
    sync_fs();
    print_fs();

    /*after that u can :*/
    mount_fs(); /*must have at init*/

    allocate_file("first");
    set_filesize(0,5000);

    char data = 'a';
    int i;
    for(i = 0; i<49; ++i)
    {
        write_byte(0,i*100, &data);
    }

    sync_fs(); /*must have at finish*/

    print_fs();
    printf("Done\n");
}
