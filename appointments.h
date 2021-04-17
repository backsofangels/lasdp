#ifndef APPOINTMENTS_H
#define APPOINTMENTS_H

#include "reservation.h"

typedef struct Day {
    //Maybe should have been an array (?)
    Reservation morning[2];
    Reservation afternoon[2];
    Reservation evening[2];
} Day;

Day disponiAppuntamenti(Reservation *reservations);
Day printAppointments(Day day);
Day deleteAppointments(int reservationId);
Day addAppointment(Reservation r);
//TODO: eliminare, solo per debug
void disponiAppuntamentiTest();
void printSingleReservationInstance(Reservation r);


#endif
