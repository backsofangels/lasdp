#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reservation.h"
#include "appointments.h"

//Init of day, use it whenever a new instance of the struct Day is needed
Day initializeDay() {
    Day d;
    for (int i = 0; i < 3; i++) {
        d.timeOfDay[i] = NULL;
    }
    return d;
}

//Automagically takes in reservations and creates appointments for the day
Day disponiAppuntamentiNew(Reservation *list) {
    Reservation *reservations = loadReservationsFromFile("reservations.txt");
    Day day = initializeDay();

    if (reservations == NULL) {
        return day;
    }

    while (list != NULL) {
        //La creazione di un nuovo puntatore a reservation è data dal fatto che ho bisogno di un dato temporaneo da tenermi in pancia
        //e che poi devo anche andare a staccare dalla sua lista precedente che inevitabilmente mi porterei dietro
        Reservation *temp = createReservation(list->fiscalCodeCustomer, list->timeOfTheDay);
        temp->reservationId = list->reservationId;
        temp->nextReservation = NULL;

        switch (list->timeOfTheDay) {
            case 1:
            case 2:
            case 3:
                if (countElementsInQueue(day.timeOfDay[(list->timeOfTheDay)-1]) < 2) {
                    day.timeOfDay[(list->timeOfTheDay)-1] = insertReservationOnEnd(day.timeOfDay[list->timeOfTheDay-1], temp);
                }
                break;
            default:
                //Not admitted case
                break;
        }
        list = list->nextReservation;
    }
    return day;
}

int countElementsInQueue(Reservation *list) {
    if (list == NULL) {
        return 0;
    } else {
        return 1 + countElementsInQueue(list->nextReservation);
    }
}
