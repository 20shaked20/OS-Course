CXX=g++
CXXFLAGS=-std=c++2a

HEADERS=stack.hpp
OBJECTS=stack.o
SOURCES=stack.cpp,client.cpp,ThreadServer.cpp malloc.cpp
OBJ_cli=client.o
OBJ_mall= malloc.o
OBJ_serv=ThreadServer.o
OBJ_test1 = client_test1.o
OBJ_test2 = client_test2.o
OBJ_test3 = client_test3.o


#run: demo
#	./$^

all: client server

tests: test client_test1 client_test2 client_test3

#stack: stack.o malloc.o $(OBJECTS)
#	$(CXX) $(CXXFLAGS) $^ -o stack

client: client.o $(OBJ_cli)
	$(CXX) $(CXXFLAGS) $^ -o client

server: ThreadServer.o stack.o malloc.o $(OBJ_serv)
	$(CXX) $(CXXFLAGS) $^ -o server -pthread

client_test1: client_test1.o $(OBJ_test1)
	$(CXX) $(CXXFLAGS) $^ -o client_test1

client_test2: client_test2.o $(OBJ_test2)
	$(CXX) $(CXXFLAGS) $^ -o client_test2

client_test3: client_test3.o $(OBJ_test3)
	$(CXX) $(CXXFLAGS) $^ -o client_test3

test: Test.o stack.o malloc.o
	$(CXX) $(CXXFLAGS) $^ -o test -pthread


tidy:
	clang-tidy $(SOURCES) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-* --warnings-as-errors=-* --

valgrind: client
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./client 2>&1 | { egrep "lost| at " || true; }

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) --compile $< -o $@

clean:
	rm -f *.o stack client server test client_test1 client_test2 client_test3