# Makefile

CC = gcc
CFLAGS =

all: clean server client

clean:
	rm -f server client

server:
	$(CC) $(CFLAGS) -o server server.c

client:
	$(CC) $(CFLAGS) -o client client.c
run: all
	gnome-terminal --tab --title="Server Terminal" --command="bash -c './server; exec bash'" \
                   --tab --title="Client Terminal" --command="bash -c './client; exec bash'"