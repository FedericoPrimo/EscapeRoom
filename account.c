#include "account.h"
#include "tuttigli.h"

/* Funzione che alloca dinamicamente una struct Account e e ritorna
il puntatore a tale struttura */
struct Account* new_account(char* email, char* passw){
    struct Account* save_ptr = malloc(sizeof(struct Account));
    strcpy(save_ptr->email, email);
    strcpy(save_ptr->passw, passw);
    save_ptr = NULL;
    return save_ptr;
}

/* Toglie ACCOUNT dalla lista e libera la memoria occupata da una struttura account*/
void del_account(struct Account** lista, struct Account* account){

    struct Account* corrente = *lista;
    struct Account* precedente = NULL;

    // Cerca l'account nella lista e quando lo trova è su CORRENTE
    while (corrente != NULL && corrente != account) {
        precedente = corrente;
        corrente = corrente->next;
    }

    // Se l'account è stato trovato, rimuovilo dalla lista
    if (corrente != NULL) {
        // Se l'account è in testa alla lista aggiorno LISTA
        if (precedente == NULL) {
            *lista = corrente->next;
        } else {
            precedente->next = corrente->next;
        }

        // Libera la memoria dell'ACCOUNT rimosso
        free(corrente);
    } else {
        printf("Account non trovato nella lista.\n");
    }
}
/* Inserisce ACCOUNT in cima alla lista*/
void ins_account(struct Account** lista, struct Account* account){

    // lista vuota
    if(*lista == NULL){
        *lista = account;
    } else { // lista non vuota
        account->next = *lista;
        *lista = account;
    }
}