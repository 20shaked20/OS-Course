CC = gcc
AR = ar
FLAGS = -Wall -g -pthread

OBJECTS_Stream = StreamServer.o
OBJECTS_Thread =  ThreadServer.o

all: StreamServer ThreadServer

StreamServer: $(OBJECTS_Stream)
	$(CC) $(FLAGS) -o StreamServer $(OBJECTS_Stream)

ThreadServer: $(OBJECTS_Thread) 
	$(CC) $(FLAGS) -o ThreadServer $(OBJECTS_Thread)

#Make commands as singles:

StreamServer.o: StreamServer.c
	$(CC) $(FLAGS) -c StreamServer.c

ThreadServer.o: ThreadServer.c
	$(CC) $(FLAGS) -c ThreadServer.c

.PHONY: clean all

clean:
	rm -f *.o *.so ThreadServer StreamServer

