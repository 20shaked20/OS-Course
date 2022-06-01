CC=gcc
CCX = g++

all:clean client_queue pollclient main1 guard singleton pollserver

# Q1,Q2,Q3 #
main1: main1.o
	$(CC) $(CFlags) -o $@  $^ -lpthread

client_queue: client_queue.o
	$(CC) $(CFlags) -o $@  $^ -lpthread

# Q4 #
guard:guard.cpp
	$(CCX) $(CFlags) -o guard guard.cpp -lpthread 

# Q5 #
singleton: singleton.o
	$(CCX) -o singleton singleton.cpp $(CFlags)

# Q6 #
pollclient: pollclient.o
	$(CCX) $(CFlags) -o $@  $^ -lpthread

pollserver: pollserver.o
	$(CXX) $(CFlags) -o $@  $^ -lpthread

# SHARED LIB #
# We couldnt compile it correctly after searching the web thourgly there is no way to compile into a one libary
# multiple c files with cpp files that includes multiple mains. so we cannot do so.
# if you find a solution, fix it.

# EXTRAS #
%: %.c
	$(CC) $(CFlags) -c -o $@  $^ -lpthread

%: %.cpp
	$(CC) $(CFlags) -c -o $@  $^ -lpthread

clean:
	rm -f *.dSYM *.o main1 client_queue guard singleton pollclient pollserver