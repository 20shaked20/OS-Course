# File System
In this project we created a simple file system in c. </br>

## Introduction :
to begin with, we created a data structure to support the foundation of the files under the name ``myfs.c``. </br>
our data structure is based on blocks : disk blocks & inodes. </br>
moving on, we created the ``mystdio.c``, where we used the implemenations of the data structure to implement ``myFILE`` methods. </br>

## How To Run:
- Clone the projet to a linux environment. </br>
- Open a terminal and navigate into the corresponding folder ``FileSystem``. </br>
- run the command ``make`` in the terminal in order to compile and create the entire project. </br>

you must enter this in the correct order : </br>
- under a single terminal write the following (in order to run the program) : </br>
- 1. ``./mymkfs`` - this will create the filesystem under the name ``fs_data``. </br>
- after finished, run the second program : </br>
- 2. ``./myFILE`` - this will mount the file system, and will demonstarte the methods we implemented. </br>



## Reading Material:

- [open](https://man7.org/linux/man-pages/man2/open.2.html)
- [files in c](https://www.programiz.com/c-programming/c-file-input-output)
- [readdir](https://man7.org/linux/man-pages/man3/readdir.3.html)
- [fopen](https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm)
