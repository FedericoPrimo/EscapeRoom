#ifndef ROOM_H
#define ROOM_H
int NUM_STANZE;

struct Teatro1{
    struct Tende *tende;
    struct Tribuna *tribuna;
    struct Palco *palco;
    struct Balcone *balcone;
};

struct Teatro2{
    struct Tribuna *tribuna;
    struct Balcone *balcone;
};

struct Tende{

};

struct Tribuna{

};

struct Palco{

};

struct Balcone{

};
struct Teatro1* new_teatro1();
struct Tende* new_tende();
struct Tribuna* new_tribuna();
struct Palco* new_palco();
struct Balcone* new_balcone();

void dealloca_teatro1(struct Teatro1* teatro);
void dealloca_teatro2(struct Teatro2* teatro);

#endif