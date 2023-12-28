#include "comandi_server.h"

/* Si occupa del protocollo di login con il client, restituisce l'ID dell'account associato al client
    Se account già online ritorna -1*/
int comando_login(int sd, struct Account **lista){
    int ret;
    char buf[256];
    char email[30];
    char passw[20];
    char dati[2];

    // Email è massimo 29+\0 caratteri
    ret = recv(sd, buf, 30, 0);
    if(ret == -1){
        perror("Errore nella ricezione dell'email");
    }
    printf("Email ricevuta\n");
    strncpy(email, buf, 30);

    // Password è massimo 19+\0 caratteri
    ret = recv(sd, buf, 20, 0);
    if(ret == -1){
        perror("Errore nella ricezione della password");
    }
    printf("Password ricevuta\n");
    strncpy(passw, buf, 20);
    struct Account *temp = check_account(lista, &email, &passw);

    //Controllo se è presente l'account
    if(temp != NULL){
        // Se è presente login

        // Ma se è già online ritorno -1
        if(temp->status == online){
            return -1;
        }

        strcpy(dati, "1");
        ret = send(sd, dati, sizeof(dati), 0);
        if(ret == -1){
            perror("Errore nella send della comando_login");
            exit(1);
        }
        printf("Accesso da parte dell'account con id: %d\n", temp->id);
    } else {
        // Non è presente e registro automaticamente
        strcpy(dati, "0");
        ret = send(sd, dati, sizeof(dati), 0);
        if(ret == -1){
            perror("Errore nella send della comando_login");
            exit(1);
        }
        temp = new_account(email, passw);
        ins_account(lista, temp);
        printf("Nuovo account con id: %d\n", temp->id);
        temp->status = online;
    }

    return temp->id;
}

/* Si occupa di gestire il comando "scene" del client*/
void comando_scene(){
    
}