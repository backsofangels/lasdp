#ifndef RESERVATION_H
#define RESERVATION_H

#include<stdio.h>

typedef struct Reservation {
    int reservationId;
    char fiscalCodeCustomer[17];
    int timeOfTheDay; //1 for morning, 2 for afternoon, 3 for evening
    struct Reservation *nextReservation;
} Reservation;

typedef struct Reservation* PtrReservation;

Reservation *createReservation(char *customerFiscalCode, int timeOfTheDay);
Reservation *performReservation(Reservation *r, char *customerFiscalCode);
Reservation *insertReservationOnEnd(Reservation *head, Reservation *r);
Reservation *mergeReservationLists(Reservation *symptomatics, Reservation *asymptomatics);
Reservation *deleteReservation(Reservation *r, int reservationId, int *hasCancelled);
int checkReservationAlreadyPerformed(char *fiscalCode);
void saveReservationOnFile(Reservation *headOfReservation, char *filename, char *mode);
void printReservations(Reservation *r, FILE *file, int printOnFile);
void printReservationByName(PtrReservation head, char *fiscalCode);
void printSingleReservation(PtrReservation head);
Reservation *searchReservationByTimeOfDay(Reservation *head, int timeOfTheDay);
int searchReservationByFiscalCode(PtrReservation head, char *fiscalCode);
Reservation *loadReservationsFromFile(char *filename);
void printMergedListsOnFileWrapper();
#endif
