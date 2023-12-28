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
    int id[20]; //array contenente gli id dell'account dei client
    char buf[20], comando[6];
    bool acceso = 0;
    fd_set read_fs;
    fd_set master;
    uint16_t porta;
    char dati[2];
    struct Account* lista_account = NULL;
    
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

            if(FD_ISSET(i, &read_fs)){ // Se è pronto in lettura stdin, il socket listener o uno di scambio dati

                if(i == STDIN_FILENO){

                    // Comando inserito nella console del server
                    // Gestione input
                    fgets(buf, sizeof(buf), stdin);

                    // So di aspettarmi un comando di max 5 caratteri e un intero su 16 bit
                    ret = sscanf(buf, "%5s %hu", comando, &porta);
                    porta = htons(porta);

                    if (ret == 2 && !strcmp(comando, "start")) {
                        // start <port>, inserisco nell'fd_set il descrittore per un nuovo server
                        
                        printf("Avvio del server di gioco...\n");

                        // Se server già acceso non fa nulla
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
                        ret = listen(sd_game, 10);
                        if(ret == -1){
                            perror("Errore nella listen del server");
                            exit(1);
                        }
                        printf("Socket in ascolto, è possibile collegarsi alla porta %d per giocare.\n", ntohs(porta));
                        
                        acceso = true; // setto il server come acceso per evitare di riaccenderlo.
                            
                    } else if (ret == 1 && !strcmp(comando, "stop")){
                        // stop
                        // metti roba, controlla se partite in corso.
                        if(!client_connessi){
                            printf("Arresto del server, spero di averti intrattenuto con l' Escape Room!\n");
                            close(sd_game);
                            exit(0);
                        }

                    } else {
                        // Il comando inserito nella console non è supportato.
                        printf("Comando inesistente.\n");
                        mostra_comandi_console();
                    }
                    printf("\n");

                } else if(i == sd_game){
                    // Richiesta di connessione.
                    len = sizeof(client_addr);

                    // Le richieste arrivano quando un nuovo giocatore vuole connettersi.
                    new_sd = accept(sd_game, (struct sockaddr*)&client_addr, (socklen_t*)&len);
                    if(new_sd == -1){
                        perror("Errore nell'accept");
                        close(new_sd);
                        continue;
                    }
                    printf("Connessione accettata sulla porta %d\n", ntohs(porta));

                    /* tengo traccia dei client connessi, mi serve per il comando stop. */
                    client_connessi++;

                    // Inserisco il nuovo socket nel set.
                    FD_SET(new_sd, &master);
                    if(new_sd > fd_max){
                        fd_max = new_sd; // fd_max contiene ora il valore massimo dei descrittori di socket master.
                    }
                }
                else{

                    // Un giocatore ha immesso un comando, lo identifico dato che è sempre su 5 caratteri.
                    ret = recv(i, comando, sizeof(comando), 0);
                    if(!ret){

                        // Qua il server si aspettava un comando ma si è chiuso il client e ret == 0.
                        // Lo tolgo correttamente dal set e chiudo il socket.
                        printf("Socket del client chiuso, procedo a chiudere il socket %d.\n", i);
                        close(i);
                        FD_CLR(i, &master);
                        client_connessi--;
                        printf("Socket %d chiuso, continuo...\n", i);
                        continue;
                        
                    }
                    if(ret == -1){
                        // Nel caso in cui fallisca la recv.
                        perror("Errore in fase di ricezione da un client");
                        continue;
                    }

                    // Comando login del client.
                    if(!strcmp(comando, "login")){

                        // Salvo l'ID dell'account in un array di ID.
                        id[i] = comando_login(i, &lista_account);

                        // Se è già online chiudo la connessione.
                        // Lo tolgo correttamente dal set e chiudo il socket.
                        if(id[i] = -1){
                            printf("Client già online, procedo a chiudere il socket %d.\n", i);
                            strcpy(dati, "2");
                            send(i, dati, sizeof(dati), 0);
                            close(i);
                            FD_CLR(i, &master);
                            client_connessi--;
                            printf("Socket %d chiuso, continuo...\n", i);
                        }
                    }
                    // Comando per decidere lo scenario
                    if(!strcmp(comando, "scene")){
                        comando_scene();
                    }
                }
            }
        }
    }
}
