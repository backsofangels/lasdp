#ifndef APPOINTMENTS_H
#define APPOINTMENTS_H

#include "reservation.h"

typedef struct Day {
    PtrReservation timeOfDay[3];
} Day;

Day initializeDay();
Day disponiAppuntamentiNew();
int countElementsInQueue(Reservation *resLis);
void printDailyAppointmentsOnFile(Day day, int timeOfDay, FILE *file, int printOnFile);
void printDailyAppointmentsWrapper(int printOnFile, Day day);
Day removeAppointmentById(Day day, int id);
Day addAppointmentById();
void printAppointmentByFiscalCode(Day day, char *fiscalCode);

#endif
