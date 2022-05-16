CXX=gcc
CXXFLAGS=-std=c++2a

HEADERS=stack.hpp
OBJECTS=stack.o
SOURCES=stack.cpp,client.cpp,ProcessServer.cpp malloc.cpp
OBJ_cli=client.o
OBJ_mall= malloc.o
OBJ_serv=ProcessServer.o


#run: demo
#	./$^

all: client server

#stack: stack.o malloc.o $(OBJECTS)
#	$(CXX) $(CXXFLAGS) $^ -o stack

client: client.o $(OBJ_cli)
	$(CXX) $(CXXFLAGS) $^ -o client

server: ProcessServer.o stack.o $(OBJ_serv)
	$(CXX) $(CXXFLAGS) $^ -o server -pthread

test: Test.o stack.o
	$(CXX) $(CXXFLAGS) $^ -o test -pthread


tidy:
	clang-tidy $(SOURCES) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-* --warnings-as-errors=-* --

valgrind: client
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./client 2>&1 | { egrep "lost| at " || true; }

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) --compile $< -o $@

clean:
	rm -f *.o stack client server test