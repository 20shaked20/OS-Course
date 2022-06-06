/*
 * Substitute for real dirent structure.  Note that `d_name' field is a
 * true character array although we have it copied in the implementation
 * dependent data.  We could save some memory if we had declared `d_name'
 * as a pointer refering the name within implementation dependent data.
 * We have not done that since some code may rely on sizeof(d_name) to be
 * something other than four.  Besides, directory entries are typically so
 * small that it takes virtually no time to copy them from place to place.
 */
typedef struct dirent
{
    char d_name[256];
} dirent;


/* DIR substitute structure containing directory name.  The name is
 * essential for the operation of ``rewinndir'' function. */
typedef struct DIR
{
    char *dirname;     /* directory being scanned */
    dirent current;    /* current entry */
    int dirent_filled; /* is current un-processed? */
} DIR;
/* supply prototypes for dirent functions */
static DIR *opendir(const char *dirname);
static struct dirent *readdir(DIR *dirp);
static int closedir(DIR *dirp);

/*
 * Implement dirent interface as static functions so that the user does not
 * need to change his project in any way to use dirent function.  With this
 * it is sufficient to include this very header from source modules using
 * dirent functions and the functions will be pulled in automatically.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

/*
 * <function name="opendir">
 * <intro>open directory stream for reading
 * <syntax>DIR *opendir (const char *dirname);
 *
 * <desc>Open named directory stream for read and return pointer to the
 * internal working area that is used for retrieving individual directory
 * entries.  The internal working area has no fields of your interest.
 *
 * <ret>Returns a pointer to the internal working area or NULL in case the
 * directory stream could not be opened.  Global `errno' variable will set
 * in case of error as follows:
 *
 * <table>
 * [EACESS  |Permission denied.
 * [EMFILE  |Too many open files used by the process.
 * [ENFILE  |Too many open files in system.
 * [ENOENT  |Directory does not exist.
 * [ENOMEM  |Insufficient memory.
 * [ENOTDIR |dirname does not refer to directory.  This value is not
 *           reliable on MS-DOS and MS-Windows platforms.  Many
 *           implementations return ENOENT even when the name refers to a
 *           file.]
 * </table>
 * </function>
 */
static DIR *opendir(const char *dirname)
{
    DIR *dirp;
    assert(dirname != NULL);

    dirp = (DIR *)malloc(sizeof(struct DIR));
    if (dirp != NULL)
    {
        char *p;

        /* allocate room for directory name */
        dirp->dirname = (char *)malloc(strlen(dirname) + 1 + strlen("\\*.*"));
        if (dirp->dirname == NULL)
        {
            /* failed to duplicate directory name.  errno set by malloc() */
            free(dirp);
            return NULL;
        }
        /* Copy directory name while appending directory separator and "*.*".
         * Directory separator is not appended if the name already ends with
         * drive or directory separator.  Directory separator is assumed to be
         * '/' or '\' and drive separator is assumed to be ':'. */
        strcpy(dirp->dirname, dirname);
        p = strchr(dirp->dirname, '\0');
        if (dirp->dirname < p &&
            *(p - 1) != '\\' && *(p - 1) != '/' && *(p - 1) != ':')
        {
            strcpy(p++, "\\");
        }
        /* open stream */
        if (_initdir(dirp) == 0)
        {
            /* initialization failed */
            free(dirp->dirname);
            free(dirp);
            return NULL;
        }
    }
    return dirp;
}

/*
 * <function name="readdir">
 * <intro>read a directory entry
 * <syntax>struct dirent *readdir (DIR *dirp);
 *
 * <desc>Read individual directory entry and return pointer to a structure
 * containing the name of the entry.  Individual directory entries returned
 * include normal files, sub-directories, pseudo-directories "." and ".."
 * and also volume labels, hidden files and system files in MS-DOS and
 * MS-Windows.   You might want to use stat(2) function to determinate which
 * one are you dealing with.  Many dirent implementations already contain
 * equivalent information in dirent structure but you cannot depend on
 * this.
 *
 * The dirent structure contains several system dependent fields that
 * generally have no interest to you.  The only interesting one is char
 * d_name[] that is also portable across different systems.  The d_name
 * field contains the name of the directory entry without leading path.
 * While d_name is portable across different systems the actual storage
 * capacity of d_name varies from system to system and there is no portable
 * way to find out it at compile time as different systems define the
 * capacity of d_name with different macros and some systems do not define
 * capacity at all (besides actual declaration of the field). If you really
 * need to find out storage capacity of d_name then you might want to try
 * NAME_MAX macro. The NAME_MAX is defined in POSIX standard althought
 * there are many MS-DOS and MS-Windows implementations those do not define
 * it.  There are also systems that declare d_name as "char d_name[1]" and
 * then allocate suitable amount of memory at run-time.  Thanks to Alain
 * Decamps (Alain.Decamps@advalvas.be) for pointing it out to me.
 *
 * This all leads to the fact that it is difficult to allocate space
 * for the directory names when the very same program is being compiled on
 * number of operating systems.  Therefore I suggest that you always
 * allocate space for directory names dynamically.
 *
 * <ret>
 * Returns a pointer to a structure containing name of the directory entry
 * in `d_name' field or NULL if there was an error.  In case of an error the
 * global `errno' variable will set as follows:
 *
 * <table>
 * [EBADF  |dir parameter refers to an invalid directory stream.  This value
 *          is not set reliably on all implementations.]
 * </table>
 * </function>
 */
static struct dirent *readdir(DIR *dirp)
{
    assert(dirp != NULL);
    if (dirp == NULL)
    {
        errno = EBADF;
        return NULL;
    }

    if (dirp->dirent_filled != 0)
    {
        /*
         * Directory entry has already been retrieved and there is no need to
         * retrieve a new one.  Directory entry will be retrieved in advance
         * when the user calls readdir function for the first time.  This is so
         * because real dirent has separate functions for opening and reading
         * the stream whereas Win32 and DOS dirents open the stream
         * automatically when we retrieve the first file.  Therefore, we have to
         * save the first file when opening the stream and later we have to
         * return the saved entry when the user tries to read the first entry.
         */
        dirp->dirent_filled = 0;
    }
    else
    {
        /* fill in entry and return that */
        _setdirname(dirp);
        assert(dirp->dirent_filled == 0);
    }
    return &dirp->current;
}

/*
 * <function name="closedir">
 * <intro>close directory stream.
 * <syntax>int closedir (DIR *dirp);
 *
 * <desc>Close directory stream opened by the `opendir' function.  Close of
 * directory stream invalidates the DIR structure as well as previously read
 * dirent entry.
 *
 * <ret>The function typically returns 0 on success and -1 on failure but
 * the function may be declared to return void on same systems.  At least
 * Borland C/C++ and some UNIX implementations use void as a return type.
 * The dirent wrapper tries to define VOID_CLOSEDIR whenever closedir is
 * known to return nothing.  The very same definition is made by the GNU
 * autoconf if you happen to use it.
 *
 * The global `errno' variable will set to EBADF in case of error.
 * </function>
 */
closedir(DIR *dirp)
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
