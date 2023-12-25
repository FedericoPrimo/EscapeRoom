#ifndef UTILITY_H
#define UTILITY_H
#include "tuttigli.h"

// inizializzazione
void mostra_comandi_console();
int creazione_sock_server(struct sockaddr_in *server_address, int porta);
int creazione_indirizzo_server(struct sockaddr_in *server_addr, int porta);

// identificazione
int manda_informazioni(unsigned char *email, unsigned char *passw);

#endif