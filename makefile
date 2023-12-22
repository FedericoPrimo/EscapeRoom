# Makefile

CC = gcc
CFLAGS = -Wall -c

all: server client

server: server.o utility.o
	$(CC) -Wall utility.o server.o -o server

client: client.o utility.o
	$(CC) -Wall client.o utility.o -o client

server.o: server.c
	$(CC) $(CFLAGS) server.c -o server.o

client.o: client.c
	$(CC) $(CFLAGS) client.c -o client.o

utility.o: utility.c utility.h
	$(CC) $(CFLAGS) utility.c -o utility.o

clean:
	rm *.o server client

run: all
	gnome-terminal --tab --title="Server Terminal" --command="bash -c './server 4242; exec bash'" \
                   --tab --title="Client Terminal" --command="bash -c './client 4242; exec bash'"