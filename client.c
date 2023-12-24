#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

/*
struct sockaddr_in {
    sa_family_t     sin_family; // Address family, si mette AF_INET
    in_port_t       sin_port;   // network order
    struct in_addr  sin_addr;   // indirizzo
}

struct in_addr {
    uint32_t s_addr;
};
*/

int main(int argc, char* argv[]){
    int ret, sd;
    uint16_t porta;
    char buf[64];
    struct sockaddr_in my_addr, server_addr;

    // Verifica che ci sia un argomento per la porta
    if (argc != 2) {
        printf("Inserire correttamente la porta.\n\n\tSintassi: ./client <porta>\n\n");
        exit(1);
    }
    porta = htons(atoi(argv[1]));

    // creazione socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1){
        perror("Errore");
        exit(1);
    }

    // Creazione indirizzo server
    memset(&server_addr, 0, addr_len);
    server_addr.sin_family = AF_INET;
    porta = htons(4242);
    server_addr.sin_port = porta;
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    printf("Tentativo di connessione con il server\n");
    ret = connect(sd, (struct sockaddr*)&server_addr, addr_len);
    if(ret == -1){
        perror("Errore");
        exit(1);
    }
    printf("Connessione Stabilita con porta: %d\n", ntohs(porta));
    while(1){
        printf("Messaggio: ");
        scanf("%5s", buf);

        if(!strcmp("quit!", buf)){
            break;
        }
        ret = send(sd, (void*)buf, msg_len, 0);
        
        if(ret == -1){
            perror("Errore");
            exit(1);
        }
        printf("Messaggio inviato: %s\n", buf);

        ret = recv(sd, (void*)&buf, msg_len, 0);
        if(!ret){
            printf("socket remoto chiuso.\n");
            break;
        }
        if(ret == -1){
            perror("Errore");
            exit(1);
        }
        printf("Messaggio ricevuto: %s\n", buf);

    }
    close(sd);
    exit(0);

}