#ifndef RESERVATION_H
#define RESERVATION_H

#include<stdio.h>

typedef struct Reservation {
    int reservationId;
    char fiscalCodeCustomer[17];
    int timeOfTheDay; //1 for morning, 2 for afternoon, 3 for evening
    struct Reservation* nextReservation;
} Reservation;

typedef struct Reservation* PtrReservation;

PtrReservation createReservation(char *customerFiscalCode, int timeOfTheDay);
PtrReservation performReservation(PtrReservation r, char *customerFiscalCode);
PtrReservation insertReservationOnEnd(PtrReservation head, PtrReservation r);
PtrReservation mergeReservationLists(PtrReservation symptomatics, PtrReservation asymptomatics);
PtrReservation deleteReservation(PtrReservation r, int reservationId, int *hasCancelled);
int checkReservationAlreadyPerformed(char *fiscalCode);
void saveReservationOnFile(PtrReservation headOfReservation, char *filename, char *mode);
void printReservations(PtrReservation r, FILE *file, int printOnFile);
void printReservationByName(PtrReservation head, char *fiscalCode);
void printSingleReservation(PtrReservation head);
PtrReservation searchReservationByTimeOfDay(PtrReservation head, int timeOfTheDay);
int searchReservationByFiscalCode(PtrReservation head, char *fiscalCode);
PtrReservation loadReservationsFromFile(char *filename);
void printMergedListsOnFileWrapper();
#endif
