# Makefile

CC = gcc
CFLAGS = -Wall -c

all: server client

server: server.o utility.o account.o comandi_server.o comandi_client.o sessione.o
	$(CC) -Wall utility.o server.o account.o comandi_client.o comandi_server.o sessione.o -o server

client: client.o utility.o account.o comandi_client.o comandi_server.o sessione.o
	$(CC) -Wall client.o utility.o account.o comandi_client.o comandi_server.o sessione.o -o client

server.o: server.c
	$(CC) $(CFLAGS) server.c -o server.o

client.o: client.c
	$(CC) $(CFLAGS) client.c -o client.o

utility.o: utility.c utility.h
	$(CC) $(CFLAGS) utility.c -o utility.o

account.o: account.c account.h
	$(CC) $(CFLAGS) account.c -o account.o
	
comandi_server.o: comandi_server.c comandi_server.h
	$(CC) $(CFLAGS) comandi_server.c -o comandi_server.o

comandi_client.o: comandi_client.c comandi_client.h
	$(CC) $(CFLAGS) comandi_client.c -o comandi_client.o

sessione.o: sessione.c sessione.h
	$(CC) $(CFLAGS) sessione.c -o sessione.o

clean:
	rm *.o server client

run: all
	gnome-terminal --tab --title="Server Terminal" --command="bash -c './server; exec bash'" \
                   --tab --title="Client Terminal" --command="bash -c './client 4242; exec bash'" \
				   --tab --title="Client Terminal" --command="bash -c './client 4242; exec bash'"