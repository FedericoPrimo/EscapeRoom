#include "tuttigli.h"

/* Funzione che alloca dinamicamente una struct sessione e e ritorna
il puntatore a tale struttura */
struct Sessione* new_sessione(int id, int room){
    struct Sessione* save_ptr = malloc(sizeof(struct Sessione));
    save_ptr->id = id;
    save_ptr->room = room;
    memset(save_ptr->token, 0, sizeof(save_ptr->token));
    return save_ptr;
}

/* Data una lista, controlla se è già presente una sessione per un dato ID */
struct Sessione* check_sessione(struct Sessione** lista, int id){
    if(*lista == NULL){
        printf("Sessione non presente nella lista.\n");
        return NULL;
    }
    struct Sessione* corrente = *lista;

    // Cerca la sessione nella lista
    while(corrente != NULL){
        if(corrente->id == id){
            printf("Sessione di %d trovata", id);
            return corrente;
        }
        corrente = corrente->next;
    }

    // Sessione non trovata nella lista
    printf("Sessione non trovata nella lista.\n");
    return NULL;

}

/* Inserisce SESSIONE in cima alla lista */
void ins_sessione(struct Sessione** lista, struct Sessione* sessione){

    // lista vuota
    if(*lista == NULL){
        *lista = sessione;
    } else { // lista non vuota
        sessione->next = *lista;
        *lista = sessione;
    }
}

/* Toglie SESSIONE dalla lista e libera la memoria occupata da una struttura sessione*/
void del_sessione(struct Sessione** lista, struct Sessione* sessione){

    struct Sessione* corrente = *lista;
    struct Sessione* precedente = NULL;

    // Cerca la sessione nella lista e quando lo trova è su CORRENTE
    while (corrente != NULL && corrente != sessione) {
        precedente = corrente;
        corrente = corrente->next;
    }

    // Se la sessione è stata trovata, rimuovila dalla lista
    if (corrente != NULL) {
        // Se la sessione è in testa alla lista aggiorno LISTA
        if (precedente == NULL) {
            *lista = corrente->next;
        } else {
            precedente->next = corrente->next;
        }

        // Libera la memoria della sessione rimossa
        free(corrente);
    } else {
        printf("Sessione non trovata nella lista.\n");
    }
}