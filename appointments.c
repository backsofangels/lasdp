#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reservation.h"
#include "appointments.h"

Day disponiAppuntamenti(Reservation *reservation) {
    Day d;
    int i = 0;
    for (i = 0; i < 2; i++) {
        Reservation r;
        Reservation *res = searchReservationByTimeOfDay(reservation, 1);
        r.reservationId = res->reservationId;
        strcpy(r.fiscalCodeCustomer, res->fiscalCodeCustomer);
        r.timeOfTheDay = res->timeOfTheDay;
    }

    for(i = 0; i < 2; i++) {
        Reservation r;
        Reservation *res = searchReservationByTimeOfDay(reservation, 2);
        r.reservationId = res->reservationId;
        strcpy(r.fiscalCodeCustomer, res->fiscalCodeCustomer);
        r.timeOfTheDay = res->timeOfTheDay;
    }

    for(i = 0; i < 2; i++) {
        Reservation r;
        Reservation *res = searchReservationByTimeOfDay(reservation, 3);
        r.reservationId = res->reservationId;
        strcpy(r.fiscalCodeCustomer, res->fiscalCodeCustomer);
        r.timeOfTheDay = res->timeOfTheDay;
    }

    return d;
}

//TODO: Remove this function, only for testing purpose

void disponiAppuntamentiTest() {
    printf("Dispongo gli appuntamenti\n");
    Reservation *reservations = loadReservationsFromFile("reservations.txt");
    if (reservations == NULL) {
        return;
    }

    Day d = disponiAppuntamenti(reservations);

    int i = 0;
    for (i = 0; i < 2; i++) {
        printf("Morning reservations:\n");
        printSingleReservationInstance(d.morning[i]);
        printSingleReservationInstance(d.afternoon[i]);
        printSingleReservationInstance(d.evening[i]);
    }
}

void printSingleReservationInstance(Reservation r) {
    printf("\tReservation Id: %d\n", r.reservationId);
    printf("\tReservation CF: %s\n", r.fiscalCodeCustomer);
    printf("\tReservation time of the day: %d\n", r.timeOfTheDay);
}
