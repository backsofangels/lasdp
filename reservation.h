#ifndef RESERVATION_H
#define RESERVATION_H

typedef struct Reservation {
    char fiscalCodeCustomer[17];
    int timeOfTheDay; //0 for morning, 1 for afternoon, 2 for evening
    int hasSympthoms;
    struct Reservation *nextReservation;
} Reservation;

typedef struct Day {
    //Maybe should have been an array (?)
    Reservation morning[2];
    Reservation afternoon[2];
    Reservation evening[2];
} Day;

Reservation *createReservation(char *customerFiscalCode, int timeOfTheDay, int hasSympthoms);
Reservation *performReservation(Reservation *r);
Reservation *insertReservationOnEnd(Reservation *head, Reservation *r);
Reservation *insertReservationOnBeginning(Reservation *r);
void printReservations(Reservation *r);

#endif