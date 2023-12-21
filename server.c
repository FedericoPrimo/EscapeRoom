#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    int sd, new_sd, ret, len, msg_len;
    char buf[6];
    msg_len = sizeof(buf);
    pid_t pid;
    struct sockaddr_in my_addr, client_addr;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1){
        perror("Errore: creazione socket server\n");
        exit(1);
    }
    
    // inizializzazione
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_port = htons(4243);
    my_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &my_addr.sin_addr);

    // inizio vero
    ret = bind(sd, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(ret == -1){
        perror("Errore: bind server\n");
        exit(1);
    }
    ret = listen(sd, 10);
    if(ret == -1){
        perror("Errore: listen server\n");
        exit(1);
    }
    len = sizeof(client_addr);
    printf("Fine Inizializzazione\n");
    while(1){
        new_sd = accept(sd, (struct sockaddr*)&client_addr, &len);
        if(new_sd == -1){
            perror("Errore: accept server\n");
            exit(1);
        }
        pid = fork();
        if(pid == -1){
            perror("Errore: fork server\n");
            exit(1);
        }
        if(!pid){
            /* proc figlio */
            ret = close(sd);
            printf("Connessione Stabilita!\n");
            while(1){
                ret = recv(new_sd, (void*)&buf, msg_len, 0);
                if(!ret){
                    printf("socket remoto chiuso.\n");
                    break;
                }
                if(ret == -1){
                    perror("Errore: recv server\n");
                    exit(1);
                }
                printf("Messaggio ricevuto: %s\n", buf);

                printf("Messaggio: ");
                scanf("%5s", buf);

                if(!strcmp("quit!", buf)){
                    break;
                }

                ret = send(new_sd, (void*)buf, msg_len, 0);
                if(ret == -1){
                    perror("Errore: creazione socket server\n");
                    exit(1);
                }
                printf("Messaggio inviato: %s\n", buf);

            }
            close(new_sd);
            exit(0);
        }
        close(new_sd);
    }
}