# Makefile

CC = gcc
CFLAGS = -Wall -g

all: server client utility

server: utility
	$(CC) $(CFLAGS) server.c utility.o -o server

client: utility
	$(CC) $(CFLAGS) client.c utility.o -o  utility.c

utility:
	$(CC) $(CFLAGS) utility.c -c -o utility

clean:
	rm -f server client utility

run: all
	gnome-terminal --tab --title="Server Terminal" --command="bash -c './server; exec bash'" \
                   --tab --title="Client Terminal" --command="bash -c './client; exec bash'"