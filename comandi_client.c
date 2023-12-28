#include "tuttigli.h"

/* Conversione di email e password*/
int manda_informazioni(int sd, unsigned char *email, unsigned char *passw){
    char buf[256];
    char comando[6];
    strcpy(comando, "login");
    int ret;

    // mando il comando
    ret = send(sd, comando, sizeof(comando), 0);
    if(ret == -1){
        perror("Errore nella send del comando login");
    }

    // converto in binary e mando email
    strncpy(buf, email, 30);
    ret = send(sd, buf, 30, 0);
    if(ret == -1){
        perror("Errore nell'invio dell'email'");
    }

    // converto in binary e mando email
    strncpy(buf, passw, 20);
    ret = send(sd, buf, 20, 0);
    if(ret == -1){
        perror("Errore nell'invio della password");
    }
    
    return ret;
}