#ifndef ROOM_H
#define ROOM_H
int NUM_STANZE;

struct Account{
    int id;
    char email[30];
    char passw[20];
    bool status;
    struct Account* next;
};

#endif