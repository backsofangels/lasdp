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
    int timeOfDayIndex = 0;

    while (timeOfDayIndex < 3) {
        day.timeOfDay[timeOfDayIndex] = deleteReservation(day.timeOfDay[timeOfDayIndex], appointmentId);
        timeOfDayIndex++;
    }

    printDailyAppointmentsWrapper(1, day);

    return day;
}

Day addAppointmentManually(Day day) {
    char customerFiscalCode[17];
    int timeOfDay;
    int timeOfDayIndex = 0;
    int inserted = 0;

    printf("Inserisci il codice fiscale dell'utente\nper il quale vuoi inserire un appuntamento.\n");
    scanf("%16s", customerFiscalCode);
    printf("Inserisci il momento del giorno nel quale vuoi eseguire il tampone\n");
    printf("\t1. Mattina\n\t2. Pomeriggio\n\t3. Sera\n");
    scanf("%1d", timeOfDay);

    PtrReservation reservation = createReservation(customerFiscalCode, timeOfDay);

    while (timeOfDayIndex < 3 && inserted == 0) {
        if (countElementsInQueue(day.timeOfDay[timeOfDayIndex]) != 2) {
            day.timeOfDay[timeOfDayIndex] = insertReservationOnEnd(day.timeOfDay[timeOfDayIndex], reservation);
            inserted = 1;
        } else {
            timeOfDayIndex++;
        }
    }

    if (inserted != 0) {
        printDailyAppointmentsWrapper(1, day);
    }

    return day;
}

void printAppointmentByFiscalCode(Day day, char *fiscalCode) {
    int index;

    int found = 0;
    for (index = 0; index < 3; index++) {
        while(day.timeOfDay[index] != NULL && found == 0) {
            if (strcmp(day.timeOfDay[index]->fiscalCodeCustomer, fiscalCode) == 0) {
                printf("%d\n%s\n%d\n", day.timeOfDay[index]->reservationId, day.timeOfDay[index]->fiscalCodeCustomer, day.timeOfDay[index]->timeOfTheDay);
                found = 1;
            }
            day.timeOfDay[index] = day.timeOfDay[index]->nextReservation;
        }
    }
}
