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

void initDatabase() {
    FILE *genericDbTable = NULL;

    genericDbTable = fopen("reservations.txt", "r");

    if (genericDbTable == NULL) {
        genericDbTable = fopen("reservations.txt", "w");
        fclose(genericDbTable);
    }

    genericDbTable = fopen("daily-appointments.txt", "r");

    if (genericDbTable == NULL) {
        genericDbTable = fopen("daily-appointments.txt", "w");
        fclose(genericDbTable);
    }

    genericDbTable = fopen("covid-test-history.txt", "r");

    if (genericDbTable == NULL) {
        genericDbTable = fopen("covid-test-history.txt", "w");
        fclose(genericDbTable);
    }

    genericDbTable = fopen("symptomatics.txt", "r");

    if (genericDbTable == NULL) {
        genericDbTable = fopen("symptomatics.txt", "w");
        fclose(genericDbTable);
    }

    genericDbTable = fopen("symptomatics.txt", "r");

    if (genericDbTable == NULL) {
        genericDbTable = fopen("symptomatics.txt", "w");
        fclose(genericDbTable);
    }

    genericDbTable = fopen("asymptomatics.txt", "r");

    if (genericDbTable == NULL) {
        genericDbTable = fopen("asymptomatics.txt", "w");
        fclose(genericDbTable);
    }

    genericDbTable = fopen("quarantine-log.txt", "r");

    if (genericDbTable == NULL) {
        genericDbTable = fopen("quarantine-log.txt", "w");
        fclose(genericDbTable);
    }

    genericDbTable = fopen("customerRegistration.txt", "r");

    if (genericDbTable == NULL) {
        genericDbTable = fopen("customerRegistration.txt", "w");
        fclose(genericDbTable);
    }

    genericDbTable = fopen("testcenterregistration.txt", "r");

    if (genericDbTable == NULL) {
        genericDbTable = fopen("testcenterregistration.txt", "w");
        fclose(genericDbTable);
    }
}

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #endif // _WIN32

    #ifdef __linux__
    system("clear");
    #endif // __linux__
}

void waitInputPrint(char *message) {
    printf("%s\n", message);
    fflush(stdin);
    getchar();
    fflush(stdin);
}

int sanitizeUserInput(int maxUserInput) {
    int temporaryInput, userchoice = -1;

    fflush(stdin);
    if (scanf("%d", &temporaryInput) == 1) {
        fflush(stdin);
        if (temporaryInput > 0 && temporaryInput < maxUserInput) {
            userchoice = temporaryInput;
        } else {
            printf("Scelta non corretta, riprovare\n");
            waitInputPrint("Premi INVIO per tornare al menu'");
        }
    } else {
        fflush(stdin);
        printf("Scelta non corretta, riprovare\n");
        waitInputPrint("Premi INVIO per tornare");
    }
    return userchoice;
}

