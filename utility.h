#ifndef UTILITY_H
#define UTILITY_H
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void mostra_comandi_console();
int creazione_sock_server(struct sockaddr_in *server_address, int porta);

#endif