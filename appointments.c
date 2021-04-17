#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reservation.h"
#include "appointments.h"

Day disponiAppuntamenti(Reservation *reservation) {
    printf("disponiAppuntamenti()\n");
    Day d;
    int i = 0;

    //printReservations(reservation, NULL, 0);
    
    Reservation *res = NULL;
    

    //printReservations(res, NULL, 0);

    //Mi sono portato le prenotazioni per una fascia oraria in res
    while (i < 2 && res != NULL) {
        Reservation *tmp = res;
        res = searchReservationByTimeOfDay(reservation, 1);
        d.morning[i].reservationId = tmp->reservationId;
        strcpy(d.morning[i].fiscalCodeCustomer, tmp->fiscalCodeCustomer);
        d.morning[i].timeOfTheDay = tmp->timeOfTheDay;
        res = res->nextReservation;
        i++;
    }
    

    while (i < 2 && res != NULL) {
        Reservation *tmp = res;
        res = searchReservationByTimeOfDay(reservation, 2);
        d.morning[i].reservationId = tmp->reservationId;
        strcpy(d.morning[i].fiscalCodeCustomer, tmp->fiscalCodeCustomer);
        d.morning[i].timeOfTheDay = tmp->timeOfTheDay;
        res = res->nextReservation;
        i++;
    }

    while (i < 2 && res != NULL) {
        Reservation *tmp = res;
        res = searchReservationByTimeOfDay(reservation, 3);
        d.morning[i].reservationId = tmp->reservationId;
        strcpy(d.morning[i].fiscalCodeCustomer, tmp->fiscalCodeCustomer);
        d.morning[i].timeOfTheDay = tmp->timeOfTheDay;
        res = res->nextReservation;
        i++;
    }

    return d;
}

//TODO: Remove this function, only for testing purpose

void disponiAppuntamentiTest() {
    printf("disponiAppuntamentiTest()\n");
    Reservation *reservations = loadReservationsFromFile("reservations.txt");
    if (reservations == NULL) {
        return;
    }

    Day d = disponiAppuntamenti(reservations);

    int i = 0;
    printf("Reservations:\n");
    printf("Morning reservations\n");
    for (i = 0; i < 2; i++) {
        printSingleReservationInstance(d.morning[i]);
    }
    printf("afternoon reservations\n");
    for (i = 0; i < 2; i++) {
        printSingleReservationInstance(d.afternoon[i]);
    }
    printf("evening reservations\n");
    for (i = 0; i < 2; i++) {
        printSingleReservationInstance(d.evening[i]);
    }
}

void printSingleReservationInstance(Reservation r) {
    printf("\tReservation Id: %d\n", r.reservationId);
    printf("\tReservation CF: %s\n", r.fiscalCodeCustomer);
    printf("\tReservation time of the day: %d\n", r.timeOfTheDay);
}
