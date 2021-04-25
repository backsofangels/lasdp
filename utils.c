#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void emptyReservationListsFiles() {
    FILE *asymptomatics = NULL;
    FILE *symptomatics = NULL;
    asymptomatics = fopen("asymptomatics.txt", "w");
    symptomatics = fopen("symptomatics.txt", "w");
    fclose(asymptomatics);
    fclose(symptomatics);
}

void emptyCommonReservationList() {
    FILE *reservations = NULL;
    reservations = fopen("reservations.txt", "w");
    fclose(reservations);
}

void emptyAppointmentsFile() {
    FILE *appointments = NULL;

    appointments = fopen("daily-appointments.txt", "w");

    fclose(appointments);
}