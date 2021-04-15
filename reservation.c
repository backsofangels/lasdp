#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reservation.h"

Reservation *createReservation(char *customerFiscalCode, int timeOfTheDay, int hasSypmthoms) {
    Reservation *r = NULL;
    r = (struct Reservation*) malloc(sizeof(struct Reservation));
    strcpy(r->fiscalCodeCustomer, customerFiscalCode);
    r->timeOfTheDay = timeOfTheDay;
    r->hasSympthoms = hasSypmthoms;
    r->nextReservation = NULL;
    return r;
}

Reservation *performReservation(Reservation *headOfReservation, char *fiscalCode) {
    int symptoms, timeOfTheDay;
    printf("Hai sintomi?\n1. Si\n2. No\n");
    scanf("%d", &symptoms);
    printf("Quando vorresti fare il tampone?\n1. Mattina\n2. Pomeriggio\n3. Sera\n");
    scanf("%d", &timeOfTheDay);
    Reservation *r = createReservation(fiscalCode, timeOfTheDay, symptoms);
    if (symptoms == 1) {
        r->nextReservation = headOfReservation;
        headOfReservation = r;
        return headOfReservation;
        insertReservationOnBeginning(headOfReservation);
    } else if (symptoms == 2) {
        insertReservationOnEnd(headOfReservation, r);
    } else {
        printf("Questa cosa e' impossibile\n");
    }
}

Reservation *insertReservationOnEnd(Reservation *head, Reservation *r) {
    if (head == NULL) {
        return r;
    } else {
        head->nextReservation = insertReservationOnEnd(head->nextReservation, r);
    }
    return head;
}

void printReservation(Reservation *head) {
    if (head == NULL) {
        return;
    }
    printf("Reservation data:\n\n\tFiscal Code: %s\n\tTime of the day: %d\n\tHas Symptoms:%d\n", head->fiscalCodeCustomer, head->timeOfTheDay, head->hasSympthoms);
    printReservation(head->nextReservation);
}