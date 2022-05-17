
# processes : Client - Server
This is a Project for server supporting mutliple client connection over processes with a shared memory stack data structure between them. </br>


## Introduction :

In this project we were requested to create a server - client connectivtiy over processes while allowing data insert to a stack. </br>
In order to make sure our program is 'process safe' we used the fcntl lock mechanisem. </br>
Adding to that, we opened a file and saved its file descriptor to allow our data to be shared between all the processes </br>
Also, we were requested to create our own malloc() and free() to be used by our methods. </br>

## How To Run:

- Clone the projet to a linux environment. </br>
- Open a terminal and navigate into the corresponding folder ``Processces``. </br>
- run the command ``make`` in the terminal in order to create the server and the client. </br>
- in two seperate terminals write the following (in order to run the program) : </br>
- 1. ``./server`` </br>
- 2. ``./client 127.0.0.1`` </br>
- > if you wish to run the more clients to test the server, open another terminal and enter: ``./client 127.0.0.1`` </br>

We're adding a photo to explain in details : </br>
* Client - Server Demo :
<img width="1277" alt="RunDemo" src="https://user-images.githubusercontent.com/73894107/168883872-3e880b85-3015-4e6a-ac0c-0e523c39656d.png">


### Operations:
Our stack supports the following commands: </br>
``POP`` - removes the head of the stack. </br>
``Push <string> `` - adds a string to the head of the stack. </br>
``TOP`` - presents the top of the stack. </br>


## Reading Material:

- [MMAP](https://man7.org/linux/man-pages/man2/mmap.2.html)
- [FCNTL](https://man7.org/linux/man-pages/man2/fcntl.2.html)
- [PROCESSES](https://www.softwaretestinghelp.com/unix-processes/)

