#ifndef ACCOUNT_H
#define ACCOUNT_H

/* Account con login basilare costituito da email e password, organizzati in lista non ordinata */
struct Account{
    char email[30];
    char passw[20];
    struct Account* next;
};

struct Account* new_account();
void del_account(struct Account**, struct Account*);
void ins_account(struct Account**, struct Account*);

#endif