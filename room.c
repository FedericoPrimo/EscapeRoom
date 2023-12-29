#include "tuttigli.h"

/* Restituisce una nuova istanza di Teatro1*/
struct Teatro1* new_teatro(){
    struct Teatro1 *save_ptr = malloc(sizeof(struct Teatro1));
    save_ptr->balcone = new_balcone();
    save_ptr->palco = new_palco();
    save_ptr->tende = new_tende();
    save_ptr = new_tribuna();
    return save_ptr;
}

/* Allocazione di una struttura Balcone */
struct Balcone* new_balcone(){
    struct Balcone *save_ptr = malloc(sizeof(struct Balcone));

}

/* Allocazione di una struttura Palco */
struct Palco* new_palco(){
    struct Palco *save_ptr = malloc(sizeof(struct Palco));

}

/* Allocazione di una struttura Tende */
struct Tende* new_tende(){
    struct Tende *save_ptr = malloc(sizeof(struct Tende));

}

/* Allocazione di una struttura Tribuna */
struct Tribuna* new_tribuna(){
    struct Tribuna *save_ptr = malloc(sizeof(struct Tribuna));

}

/* Deallocazione della struttura Teatro e delle strutture interne*/
void dealloca_teatro1(struct Teatro1* teatro){

    free(teatro->tende);
    free(teatro->balcone);
    free(teatro->palco);
    free(teatro->tribuna);
    free(teatro);
    
}
void dealloca_teatro2(struct Teatro2* teatro){

    free(teatro->balcone);
    free(teatro->tribuna);
    free(teatro);
    
}