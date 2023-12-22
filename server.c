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
    int sd_init, new_sd, ret, len, fd_max = 0, i;
    char buf[256], comando[6];
    bool acceso = 0;
    fd_set read_fs;
    fd_set master;
    uint16_t porta;
    pid_t pid;
    struct sockaddr_in my_addr, client_addr;

    // Inizializza il set di file descriptor
    FD_ZERO(&master);
    FD_ZERO(&read_fs);

    // Aggiungi l'input standard (stdin) al set
    FD_SET(STDIN_FILENO, &master);
    
    sd_init = socket(AF_INET, SOCK_STREAM, 0);
    if(sd_init == -1){
        perror("Errore: creazione socket server\n");
        exit(1);
    }
    printf("Socket creato\n");
    
    // Inizializzazione
    memset(&my_addr, 0, sizeof(my_addr));

    // Verifica che ci sia un argomento per la porta
    if (argc != 2) {
        printf("Inserire correttamente la porta.\n\n\tSintassi: ./server <porta>\n\n");
        exit(1);
    }
    porta = htons(atoi(argv[1]));

    my_addr.sin_port = porta;
    my_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &my_addr.sin_addr);

    ret = bind(sd_init, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(ret == -1){
        perror("Errore");
        exit(1);
    }
    printf("Assegnato indirizzo al socket\n");
    ret = listen(sd_init, 10);
    if(ret == -1){
        perror("Errore");
        exit(1);
    }
    printf("Socket in ascolto, è possibile ad UN SOLO client di collegarsi e mandare comandi alla porta %d\n", ntohs(porta));
    // Inizializza il set di file descriptor
    FD_SET(sd_init, &master);
    len = sizeof(client_addr);
    if(sd_init > fd_max){
        fd_max = sd_init; // fd_max contiene ora il valore massimo dei descrittori di socket master
    }
    printf("Fine inizializzazione\n");
    // mostro la console dei comandi
    mostra_comandi();

    while(1){

        // Azzero la variabile dei comandi e buf
        memset(comando, 0, sizeof(comando));
        memset(buf, 0, sizeof(buf));

        //salvo la lista di descrittori master in un'altra lista per poi usare la select();
        read_fs = master;

        //ciclo per tutto il nuovo set contente solo i fd pronti
        select(fd_max+1, &read_fs, NULL, NULL, NULL);
        for(i = 0; i <= fd_max; i++){
            if(FD_ISSET(i, &read_fs)){ // se è pronto in lettura
                if(i == STDIN_FILENO){

                    // Comando inserito nella console del server
                    // Gestione input
                    fgets(buf, sizeof(buf), stdin);
                    sscanf(buf, "%5s %d", comando, &porta);
                    if (!strcmp(comando, "start")) {
                        // start <port>, inserisco nell'fd_set il descrittore per un nuovo server
                        if(acceso){
                            printf("Server già acceso sulla porta %d\n", ntohs(porta));
                            continue;
                        }
                        
                            
                    } else if (!strcmp(comando, "stop")){
                        // stop
                        // metti roba, controlla se partite in corso
                    } else {
                        printf("Comando inesistente.\n");
                        mostra_comandi();
                    }             
                }
                if(i == sd_init){
                    if(acceso){
                        ret = accept(sd_init, (struct sockaddr*)&client_addr, &len);
                        if(ret == -1){
                            perror("Errore");
                            exit(1);
                        }
                        printf("Connessione accettata sulla porta &d\n", ntohs(porta));
                    }
                }
            }
        }
    }
}