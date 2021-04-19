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
Day disponiAppuntamentiNew() {
    Reservation *list = loadReservationsFromFile("reservations.txt");
    Day day = initializeDay();

    if (list == NULL) {
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

void printDailyAppointmentsWrapper(int printOnFile, Day day) {
    if (printOnFile == 0) {
        printDailyAppointmentsOnFile(day, 0, NULL, 0);
    } else if (printOnFile == 1) {
        FILE *file = NULL;
        file = fopen("daily-appointments.txt", "w");
        printDailyAppointmentsOnFile(day, 0, file, printOnFile);
        fclose(file);
    }
}

void printDailyAppointmentsOnFile(Day day, int timeOfDay, FILE *file, int printOnFile) {
    if (timeOfDay == 3) {
        return;
    } else {
        printReservations(day.timeOfDay[timeOfDay], file, printOnFile);
        printDailyAppointmentsOnFile(day, timeOfDay + 1, file, printOnFile);
    }
}

Day removeAppointmentById(Day day, int appointmentId) {
    int timeOfDay = 0;

    while (timeOfDay < 3) {
        day.timeOfDay[timeOfDay] = deleteReservationNew(day.timeOfDay[timeOfDay], appointmentId);
        timeOfDay++;
    }

    return day;
}
