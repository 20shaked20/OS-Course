
# Threads : Client - Server
This is a Project for server supporting mutliple client connection over threads with stack as the data structure. </br>


## Introduction :

In this project we were requested to create a server - client connectivtiy over threads while allowing data insert to a stack. </br>
In order to make sure our program is 'thread safe' we used the Pthread mutex which functions as a LOCK. </br>
Also, we were requested to create our own malloc(), calloc() and free() to be used by our methods. </br>

## How To Run:

- Clone the projet to a linux environment. </br>
- Open a terminal and navigate into the corresponding folder ``Threads``. </br>
- run the command ``make`` in the terminal in order to create the server and the client. </br>
- in two seperate terminals write the following (in order to run the program) : </br>
- 1. ``./server`` </br>
- 2. ``./client 127.0.0.1`` </br>
- > if you wish to run the more clients to test the server, open another terminal and enter: ``./client 127.0.0.1`` </br>

We're adding a photo to explain in details : </br>
* Client - Server :
<img width="1429" alt="Screen Shot 2022-04-21 at 12 11 13" src="https://user-images.githubusercontent.com/73894107/164423777-f7762b01-1fcd-4242-8433-54d09b4c9f9e.png">

### Tests:
We added a simple assert tests checking for the stack operations. ``POP``, ``PUSH``, ``TOP``. </br>
Also we simulated a 3 client connection to a server. </br>
The simulated clients are named under ``client_test1``,``client_test2``,``client_test3``. </br>
You can see the tests in the file ``Test.cpp``. </br>
In order to compile the tests in the termianl enter: ``make tests`` it will compile everything neccessary for the tests. </br>
MAKE sure to compile ``make all`` before. </br> 
in order to run and see the tests enter in the terminal: ``./test``. </br>


## Reading Material:

- [Threads](https://www.geeksforgeeks.org/multithreading-c-2/)
- [Mutex](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [sbrk](https://linux.die.net/man/2/sbrk)
- [malloc](https://www.tutorialspoint.com/c_standard_library/c_function_malloc.htm)
- [brk](https://man7.org/linux/man-pages/man2/fork.2.html)

