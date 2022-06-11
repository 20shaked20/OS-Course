CC=gcc -fPIC -g -w

all: mymkfs myFILE libmylibc.so libmyfs.so

# Q1 #
mymkfs: myfs.o myfs_test.o
	$(CC) $(CFlags) -o $@  $^

# Q2 #
myFILE: myfs.o mystdio.o mystdio_test.o
	$(CC) $(CFlags) -o $@  $^

# SHARED LIB #
libmyfs.so: myfs.o
	$(CC) --shared -fPIC -g -pthread myfs.o -o libmyfs.so

libmylibc.so: myfs.o mystdio.o
	$(CC) --shared -fPIC -g -pthread myfs.o mystdio.o -o libmylibc.so


# EXTRAS #
%: %.c
	$(CC) $(CFlags) -c -o $@  $^ 

clean:
	rm -f *.dSYM *.o mymkfs fs_data myFILE libmyfs.so libmylibc.so