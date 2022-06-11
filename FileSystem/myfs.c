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

#include "myfs.h"

/*globals*/
struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;

/*****************************************HELPER METHODS*****************************************/
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

/*could be lseek*/
int get_block_num(int file, int offset)
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

void sync_fs()
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

void shorten_file(int bn)
{
    int nn = dbs[bn].next_block_num;
    if (nn >= 0)
    {
        shorten_file(nn);
    }
    dbs[bn].next_block_num = -1;
}

void set_filesize(int fd, int size)
{
    int tmp = size + 512 - 1;
    int num = tmp / 512;

    int bn = inodes[fd].first_block;
    num--;
    // Increase file if needed
    while (num > 0)
    {
        /*next block number*/
        int next_num = dbs[bn].next_block_num;
        if (next_num == -2)
        {
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

/*****************************************FILE SYSTEM*****************************************/
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
        strcpy(inodes[i].name, "empty file");
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
    file = fopen(source, "r");

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
    int in = find_empty_inode(sb, &inodes);
    int block = find_empty_disk_block(sb, &dbs);

    inodes[in].first_block = block;
    dbs[block].next_block_num = -2;

    strcpy(inodes[in].name, pathname);

    set_filesize(in, 5000); /*setting a file size a default size of 5000*/

    /*return fd*/
    return in;
}

int myclose(int myfd)
{
    /*TODO: check if needed to reset memory usage using the disk block*/
    struct inode *file = &inodes[myfd];
    set_filesize(myfd,0);
    file->size = -1;
    file->first_block = -1;
    strcpy(file->name, "empty file");

    return 0; /*true*/
}

ssize_t myread(int myfd, void *buf, size_t count)
{
    /*convert to char*/
    char *data;
    data = (char *)buf;

    /*which block*/
    int relative_block = count / MAXBLOCK;

    /*find block number*/
    int bn = get_block_num(myfd, relative_block);

    /*offset in block*/
    int offset = count % MAXBLOCK;

    /*prints data to stdin*/
    char tmp[512];
    while (offset >= 0)
    {
        tmp[offset] = dbs[bn].data[offset];
        --offset;
    }
    printf("%s\n", tmp);
}

ssize_t mywrite(int myfd, const void *buff, size_t count)
{

    /*convert to char*/
    char *data;
    data = (char *)buff;

    /*which block*/
    int relative_block = count / MAXBLOCK;

    /*find block number*/
    int bn = get_block_num(myfd, relative_block);

    /*offset in block*/
    int offset = count % MAXBLOCK;
    
    /*write data*/
    dbs[bn].data[offset] = (*data);
}

/*CHECK THIS*/
off_t mylseek(int myfd, off_t offset, int whence)
{
    /*which block*/
    int relative_block = whence / MAXBLOCK;

    /*find block number*/
    int bn = get_block_num(myfd, relative_block);

    /*offset in block*/
    offset = whence % MAXBLOCK;

    /*return block place after moving offset*/
    return dbs[bn].data[offset];
}

/*****************************************MY DIRENT*****************************************/

static myDIR *myopendir(const char *name)
{
	myDIR *dirp;
	assert(name != NULL);

	dirp = (myDIR *)malloc(sizeof(struct DIR));
	if (dirp != NULL)
	{
		char *p;

		/* allocate room for directory name */
		dirp->dirname = (char *)malloc(strlen(name) + 1 + strlen("\\*.*"));
		if (dirp->dirname == NULL)
		{
			/* failed to duplicate directory name.  errno set by malloc() */
			free(dirp);
			return NULL;
        }
		strcpy(dirp->dirname, name);
		p = strchr(dirp->dirname, '\0');
		if (dirp->dirname < p &&
			*(p - 1) != '\\' && *(p - 1) != '/' && *(p - 1) != ':')
		{
			strcpy(p++, "\\");
		}
		/* open stream */
		// if (_initdir(dirp) == 0)
		// {
		// 	/* initialization failed */
		// 	free(dirp->dirname);
		// 	free(dirp);
		// 	return NULL;
		// }
	}
	return dirp;
}


static struct mydirent *myreaddir(myDIR *dirp)
{
	assert(dirp != NULL);
	if (dirp == NULL)
	{
		errno = EBADF;
		return NULL;
	}

	if (dirp->dirent_filled != 0)
	{
	
		dirp->dirent_filled = 0;
	}
	else
	{
		/* fill in entry and return that */
		// _setdirname(dirp);
		assert(dirp->dirent_filled == 0);
	}
	return &dirp->current;
}


int myclosedir(myDIR *dirp)
{
	int retcode = 0;

	/* make sure that dirp points to legal structure */
	assert(dirp != NULL);
	if (dirp == NULL)
	{
		errno = EBADF;
		return -1;
	}

	/* free directory name and search handles */
	if (dirp->dirname != NULL)
		free(dirp->dirname);

	/* clear dirp structure to make sure that it cannot be used anymore*/
	memset(dirp, 0, sizeof(*dirp));

	free(dirp);
	return retcode;
}
