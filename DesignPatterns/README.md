
# DesignPatterns
This Project is about multiple design patterns implemented in C and C++. </br>


## Introduction :

This project is seprated into 4 parts: </br>
### Part A:
implementing a queue and above that a condition queue using the cond variable. </br>
using the queue, we implemented the design pattern ```Active Object``` and demonstarted how it works using a client - server model. </br>

### Part B:
implementing a self destructive lock to be known as guard.</br>

### PART C:
implementing singleton design pattern. </br>

### PART D:
implementing a reactor and later demonstarting it under the poll system call, using a client - server model. </br>

## How To Run:

- Clone the projet to a linux environment. </br>
- Open a terminal and navigate into the corresponding folder ``DesignPatterns``. </br>
- run the command ``make`` in the terminal in order to compile and create the entire project. </br>
### 4 PARTS:
`PART A ( Q: 1-3 )` : </br>
>
- in two seperate terminals write the following (in order to run the program) : </br>
- 1. ``./main1`` </br>
- 2. ``./client_queue 127.0.0.1`` </br>
- > if you wish to run the more clients to test the server, open another terminal and enter: ``./client_queue 127.0.0.1`` </br>

We're adding a photo to explain in details : </br>
* client_queue - main1 Demo :
<img width="1320" alt="Screen Shot 2022-06-01 at 13 08 57" src="https://user-images.githubusercontent.com/73894107/171381101-60e9806c-3607-4b52-856a-aa1aa7bec30e.png">

`PART B ( Q: 4 )` : </br>
>
- in a single terminal write the following : </br>
- ``./guard`` </br>
- it will demonstrate a simple test, which will show the guard lock mechanesim. </br>

`PART C ( Q: 5 )` : </br>
>
- in a single terminal write the following : </br>
- ``./singleton`` </br>
- it will demonstrate a simple test, which will show how the singleton design pattern works. </br>

`PART D ( Q: 6 )` : </br>
- in two seperate terminals write the following (in order to run the program) : </br>
- 1. ``./pollserver`` </br>
- 2. ``./pollclient 127.0.0.1`` </br>
- > if you wish to run the more clients to test the server, open another terminal and enter: ``./pollclient 127.0.0.1`` </br>

We're adding a photo to explain in details : </br>
- our demo mainly checks that each client gets his message back to him and only to him.
* pollclient - pollserver Demo :
<img width="1330" alt="Screen Shot 2022-06-01 at 13 17 55" src="https://user-images.githubusercontent.com/73894107/171382569-f4d79442-b9c3-4396-ba62-e02affb77bf2.png">




## Reading Material:

- [condition variable](https://www.educative.io/edpresso/what-are-conditional-variables-in-os)
- [queue](https://www.journaldev.com/36220/queue-in-c)
- [active object](https://en.wikipedia.org/wiki/Active_object)
- [pthread](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [lock](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [guard](https://en.wikipedia.org/wiki/Guard_(computer_science))
- [singleton](https://en.wikipedia.org/wiki/Singleton_pattern)
- [reactor](https://en.wikipedia.org/wiki/Reactor_pattern)
- [poll](https://man7.org/linux/man-pages/man2/poll.2.html)


