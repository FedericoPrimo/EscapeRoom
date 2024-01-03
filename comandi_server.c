#include "account.h"
#include "sessione.h"
#include "tuttigli.h"
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

    // Devo controllare se l'account ha già una sessione attiva
    struct Account *temp = check_account(lista, email, passw);

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

/* Si occupa di gestire il comando "room" del client */
void comando_rooms(uint8_t room, int id, struct Sessione **lista){
    // Ora devo gestire le sessioni, iniziarne una o recuperarla,
    // devo metterla se creata nella giusta lista.

    // Qua controllo se è già presente nella lista
    struct Sessione* temp = check_sessione(*lista, id);

    // Altrimenti la creo e la inserisco
    if(temp == NULL){
        temp = new_sessione(id, room);
        ins_sessione(lista, temp);
        printf("Sessione creata e inserita\n");
    }

    client_online++;
    // Da qui in poi il server si aspetta di ricevere solo comandi di gioco

}

/* Chiamata dopo un comando da input client, controlla se il giocatore ha vinto
    Se ha vinto restituisce 1, 0 altrimenti*/
bool check_vittoria(int sd, struct Sessione* sessione){
    int ret;
    uint8_t esito = sessione->token;
    
    // Vittoria
    ret = send(sd, (void *)&esito, sizeof(esito), 0);
    if(ret == -1){
        perror("Errore nell'invio dell'esito");
        exit(1);
    }

    if(esito == 2)
        return 1;
    return 0;
}

void comando_look(int sd, struct Sessione* sessione, int type){
    char buf[256];
    int ret;

    // Mi aspetto l'argomento del client
    ret = recv(sd, buf, sizeof(buf), 0);
    if(ret == -1){
        perror("Errore nell'invio dell'esito");
        exit(1);
    }
    // In base al tipo di room gestisco gli scenari
    if(type == 1){
        if(!strcmp(buf, "balcone")){

        } else if(!strcmp(buf, "palco")){

        } else if(!strcmp(buf, "tribuna")){

        } else if(!strcmp(buf, "tende")){

        } else {

        }

    } else if(type == 2){
        // Si guardano anche i flag per cambiare la frase stampata al client
        if(!strcmp(buf, "balcone")){

            strcpy(buf, "Noti un **baule** e un manichino in abiti da donna, deve essere **Giulietta**.");
            
        } else if(!strcmp(buf, "baule")){
            if(!sessione->flags[2])
                strcpy(buf, "Il baule è bloccato da un lucchetto speciale");
            else 
                strcpy(buf, "Hai già raccolto il braccio!");

        } else if(!strcmp(buf, "giulietta")){
            if(!sessione->flags[0])
                strcpy(buf, "Giulietta è rivolta verso il palco ma non sai dove stia guardando perché le manca la testa");
            else
                strcpy(buf, "Giulietta sta guardando il manichino di Romeo");

        } else if(!strcmp(buf, "palco")){

            strcpy(buf, "Di fronte a te vedi una **vetrina** portaoggetti e un manichino di un uomo, deve essere **Romeo**.");
        
        } else if(!strcmp(buf, "vetrina")){
            if(!sessione->flags[3])
                strcpy(buf, "Vedi una vetrina con una testa di un manichino al suo interno ma è chiusa");
            else
                strcpy(buf, "Hai già raccolto la testa del manichino");

        } else if(!strcmp(buf, "romeo")){
            if(!sessione->flags[1])
                strcpy(buf, "Romeo sta guardando in alto verso il balcone ma gli manca un braccio");
            else
                strcpy(buf, "Romeo è completo e continua ad osservare Giulietta");
        } else {

            strcpy(buf, "Argomento non valido, prova con una location o un oggetto");

        }

        ret = send(sd, buf, sizeof(buf), 0);
        if(ret == -1){
            perror("Errore nell'invio dell'esito");
            exit(1);
        }
    }
}

void comando_objs(int sd, struct Sessione* sessione, int type){
    char buf[256] = "";
    int ret;
    // A seconda dello scenario devo gestire diversamente il comando

    if(type == 1){

    } else {
        // Tipo 2

        // Controllo il flag associato alla sessione
        if(sessione->flags[4] == 1){
            strcat(buf, "testa\n");
        }
        // Controllo il flag associato alla sessione
        if(sessione->flags[5] == 1){
            strcat(buf, "braccio\n");
        }
    }

    // Mando tutto al client per la stampa
    ret = send(sd, buf, sizeof(buf), 0);
    if(ret == -1){
        perror("Errore nell'invio dell'esito");
        exit(1);
    }
}

void comando_take(int sd, struct Sessione* sessione, int type){
    char buf[256];
    char arg1[10];
    char risposta[20];
    int ret;

    ret = recv(sd, arg1, sizeof(arg1), 0);
    if(ret == 1){
        perror("Errore nella ricezione del parametro della take");
        exit(1);
    }

    if(type == 1){
        
    } else {

        // Nella stanza numero due possiamo usare la take solamente sul baule e sulla vetrina
        // Gli indovinelli non hanno un limite di tentativi
        if(!strcmp("baule", arg1)){
            
            // Controllo i flag per gestire i vari casi
            if(!sessione->flags[2]){
                // Devo mostrare l'enigma se il baule è bloccato (flag 2 == 0)
                strcpy(buf, "Il baule è bloccato. Devi risolvere l'enigma!\nIn quale città si trova il balcone della donna amata da Romeo?\n");

                ret = send(sd, buf, sizeof(buf), 0);
                if(ret == -1){
                    perror("Errore nella send dell'enigma del baule");
                    exit(1);
                }

                // Ci aspettiamo la risposta giusta che è giulietta
                ret = recv(sd, risposta, sizeof(risposta), 0);
                if(ret == -1){
                    perror("Errore nella recv della risposta all'enigma del baule");
                    exit(1);
                }
                printf("Risposta: %s", risposta);

                // Controllo la risposta e se giusta setto il flag
                if(!strcmp(risposta, "venezia\n")){
                    strcpy(buf, "Il lucchetto si è aperto e dentro vedi un braccio di un manichino\n");
                    sessione->flags[2] = 1;
                } else {
                    strcpy(buf, "Il lucchetto non si apre\n");
                }

                // Mandiamo l'esito dell'indovinello con la send finale
                ret = send(sd, buf, sizeof(buf), 0);
                if(ret == -1){
                    perror("Errore nella send dell'esito risposta all'enigma del baule");
                    exit(1);
                }
                printf("mandato\n");

            } else if(sessione->flags[2] && !sessione->flags[4]){
                // baule aperto e braccio non possesso
                strcpy(buf, "Hai preso il braccio\n");
                sessione->flags[4] = 1;
            } else {
                // l'altro caso rimasto è quando abbiamo già preso il braccio
                strcpy(buf, "Il baule è vuoto\n");
            }

        } else if(!strcmp("vetrina", arg1)){
            
        } else {
            strcpy(buf, "Argomento non valido\n");
        }
    }

    // Mando la stringa contenente l'esito della take
    ret = send(sd, buf, sizeof(buf), 0);
    if(ret == 1){
        perror("Errore nella send dell'esito della take");
        exit(1);
    }
}

void comando_use(int sd, struct Sessione* sessione, int type){
    char buf[256] = "";
    int ret;

    if(type == 1){

    } else {
        
    }
}