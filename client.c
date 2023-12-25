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
    char buf[256], email[30], passw[20];
    struct sockaddr_in server_addr;

    // Verifica che ci sia un argomento per la porta
    if (argc != 2) {
        printf("Inserire correttamente la porta.\n\n\tSintassi: ./client <porta>\n\n");
        exit(1);
    }
    porta = htons(atoi(argv[1]));

    // Creazione socket client
    sd = creazione_indirizzo_server(&server_addr, porta);
    printf("Inizializzata struttura per l'indirizzo del server e socket del client\n");
    
    // Connessione al server
    printf("Tentativo di connessione con il server...\n");
    ret = connect(sd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret == -1){
        perror("Errore");
        exit(1);
    }
    printf("Connessione Stabilita con porta: %d\n", ntohs(porta));
    
    // Connessione effettuata, inizia lo scambio di informazioni con il server
    printf("Per favore identificati.\n");
    printf("Email: ");
    scanf("%30s", email);
    printf("Password: ");
    scanf("%20s", passw);
    ret = manda_informazioni(&email, &passw);
    if(ret == -1){
        perror("Errore");
        exit(1);
    }

    // inizia il gioco
    while(1){

        ret = send(sd, (void*)buf, sizeof(buf), 0);
        if(ret == -1){
            perror("Errore");
            exit(1);
        }

        ret = recv(sd, (void*)&buf, sizeof(buf), 0);
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