#ifndef APPOINTMENTS_H
#define APPOINTMENTS_H

#include "reservation.h"

typedef struct Day {
    PtrReservation timeOfDay[3];
} Day;

Day initializeNewDay();
Day disponiAppuntamentiNew(Reservation *reservationList);
int countElementsInQueue(Reservation *resLis);

#endif
