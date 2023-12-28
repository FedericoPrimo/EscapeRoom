#ifndef SESSIONE_H
#define SESSIONE_H

struct Sessione{
    int id;
    int room;
    char token[10];
    struct Sessione *next;
};


struct Sessione* new_sessione(int id, int room);
struct Sessione* check_sessione(struct Sessione** lista, int id);
void ins_sessione(struct Sessione** lista, struct Sessione* sessione);
void del_sessione(struct Sessione** lista, struct Sessione* sessione);

#endif