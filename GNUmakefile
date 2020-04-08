CC=gcc
CFLAGS=-Wall

build: tema1so.o hashmap.o parser.o
	$(CC) tema1so.o hashmap.o parser.o -o so-cpp
tema1so.o: tema1so.c
	$(CC) $(CFLAGS) -c $<
hashmap.o: hashmap.c
	$(CC) $(CFLAGS) -c $<
parser.o: parser.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f *.o so-cpp
