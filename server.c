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

int main(){
    int sd_game, new_sd, ret, len, fd_max = 0, i, client_connessi = 0;
    char buf[256], comando[6];
    bool acceso = 0;
    fd_set read_fs;
    fd_set master;
    uint16_t porta;
    
    struct sockaddr_in my_addr, client_addr;
    
    // Inizializza il set di file descriptor
    FD_ZERO(&master);
    FD_ZERO(&read_fs);

    // Aggiungi l'input standard (stdin) al set
    FD_SET(STDIN_FILENO, &master);

    printf("Fine inizializzazione\n");
    // mostro la console dei comandi
    mostra_comandi_console();

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
                    porta = htons(porta);
                    if (!strcmp(comando, "start")) {
                        // start <port>, inserisco nell'fd_set il descrittore per un nuovo server
                        printf("Avvio del server di gioco...\n");
                        if(acceso){
                            printf("Server già acceso sulla porta %d\n", ntohs(porta));
                            continue;
                        }
                        sd_game = creazione_sock_server(&my_addr, porta);

                        // Metto il socket del server nel set
                        FD_SET(sd_game, &master);
                        if(sd_game > fd_max){
                            fd_max = sd_game; // fd_max contiene ora il valore massimo dei descrittori di socket master
                        }

                        // Resto in ascolto per giocatori che si vogliono connettere alla mia partita
                        ret = listen(sd_game, 2);
                        if(ret == -1){
                            perror("Errore");
                            continue;
                        }
                        printf("Socket in ascolto, è possibile collegarsi alla porta %d per giocare.\n", ntohs(porta));
                        
                            
                    } else if (!strcmp(comando, "stop")){
                        // stop
                        // metti roba, controlla se partite in corso
                        if(!client_connessi){
                            printf("Arresto del server, speriamo di averti intrattenuto con la nostra Escape Room!\n");
                            close(sd_game);
                            exit(0);
                        }

                    } else {
                        printf("Comando inesistente.\n");
                        mostra_comandi_console();
                    }             
                }
                if(i == sd_game){
                    len = sizeof(client_addr);

                    // le richieste arrivano quando un nuovo giocatore vuole connettersi
                    new_sd = accept(sd_game, (struct sockaddr*)&client_addr, &len);
                    if(new_sd == -1){
                        perror("Errore");
                        continue;
                    }
                    printf("Connessione accettata sulla porta %d\n", ntohs(porta));

                    /* tengo traccia dei client connessi, mi serve per il comando stop. */
                    client_connessi++;

                    FD_SET(new_sd, &master);
                    if(new_sd > fd_max){
                        fd_max = new_sd; // fd_max contiene ora il valore massimo dei descrittori di socket master
                    }

                    /* Da qui inizia l'escape room per il client accettato, devo mandargli la struttura per
                    caricare una partita. */
                    

                    
                }
                else{
                    // è una richiesta di un giocatore, devo prima identificarlo.
                }
            }
        }
    }
}
