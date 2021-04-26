#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "reservation.h"

Reservation *createReservation(char *customerFiscalCode, int timeOfTheDay) {
    Reservation *r = NULL;
    r = (struct Reservation*) calloc(1, sizeof(struct Reservation));
    strcpy(r->fiscalCodeCustomer, customerFiscalCode);
    r->reservationId = rand() % 200;
    r->timeOfTheDay = timeOfTheDay;
    r->nextReservation = NULL;
    return r;
}

//Vanno usate due liste, una per i sintomatici, una per gli asintomatici entrambe con inserimento in coda. Alla fine, le unisco con sint -> asint
Reservation *performReservation(Reservation *headOfReservation, char *fiscalCode) {
    int timeOfTheDay;
    printf("Quando vorresti fare il tampone?\n1. Mattina\n2. Pomeriggio\n3. Sera\n");
    scanf("%d", &timeOfTheDay);
    Reservation *r = createReservation(fiscalCode, timeOfTheDay);
    printf("\nHai creato la prenotazione, questi sono i dati\n");
    printSingleReservation(r);
    return insertReservationOnEnd(headOfReservation, r);
}

Reservation *insertReservationOnEnd(Reservation *head, Reservation *r) {
    if (head == NULL) {
        return r;
    } else {
        head->nextReservation = insertReservationOnEnd(head->nextReservation, r);
    }
    return head;
}

Reservation *deleteReservation(Reservation *r, int reservationId, int *hasCancelled) {
    if (r == NULL) {
        return r;
    }
    if (r->reservationId == reservationId) {
        Reservation *tmp = r->nextReservation;
        r->nextReservation = NULL;
        free(r);
        *hasCancelled = 1;
        return tmp;
    } else r->nextReservation = deleteReservation(r->nextReservation, reservationId, hasCancelled);
    return r;
}

int checkReservationAlreadyPerformed(char *fiscalCode) {
    Reservation *syntomatic = NULL;
    Reservation *asyntomatic = NULL;

    syntomatic = loadReservationsFromFile("symptomatics.txt");
    asyntomatic = loadReservationsFromFile("asymptomatics.txt");

    return searchReservationByFiscalCode(syntomatic, fiscalCode) || searchReservationByFiscalCode(asyntomatic, fiscalCode);
}

Reservation *mergeReservationLists(Reservation *symptomatics, Reservation *asymptomatics) {
    if (symptomatics == NULL) {
        return asymptomatics;
    }
    Reservation *mergedList = symptomatics;
    while (symptomatics->nextReservation != NULL) {
        symptomatics = symptomatics->nextReservation;
    }
    symptomatics->nextReservation = asymptomatics;
    return mergedList;
}

void saveReservationOnFile(Reservation *headOfReservation, char *filename, char *mode) {
    FILE *file = NULL;
    file = fopen(filename, mode);

    if (file == NULL) {
        printReservations(headOfReservation, NULL, 0);
    } else if (file != NULL) {
        printReservations(headOfReservation, file, 1);
    }

    fclose(file);
}

Reservation *searchReservationByTimeOfDay (Reservation *headOfReservation, int timeOfTheDay) {
    Reservation *localHead = headOfReservation;
    Reservation *results = NULL;

    while (localHead != NULL) {
            if (localHead->timeOfTheDay == timeOfTheDay) {
                Reservation *temp = createReservation(localHead->fiscalCodeCustomer, timeOfTheDay);
                temp->reservationId = localHead->reservationId;
                results = insertReservationOnEnd(results, temp);
            }
        localHead = localHead->nextReservation;
    }

    return results;
}

int searchReservationByFiscalCode(PtrReservation head, char *fiscalCode) {
    if (head == NULL) {
        return 0;
    }
    if (strcasecmp(head->fiscalCodeCustomer, fiscalCode) == 0 ) {
        return 1;
    } else {
        return searchReservationByFiscalCode(head->nextReservation, fiscalCode);
    }
}

void printReservations(Reservation *head, FILE *file, int printOnFile) {
    if (head == NULL) {
        return;
    }
    if (file != NULL && printOnFile == 1) {
        fprintf(file, "%d\t%s\t%d\n", head->reservationId, head->fiscalCodeCustomer, head->timeOfTheDay);
    } else if (printOnFile == 0) {
        printSingleReservation(head);
    }
    printReservations(head->nextReservation, file, printOnFile);
}

void printReservationByName(PtrReservation head, char *fiscalCode) {
    if (head == NULL) {
        return;
    }
    if (head != NULL) {
        if (strcmp(head->fiscalCodeCustomer, fiscalCode) == 0) {
            printSingleReservation(head);
        }
    }
    printReservationByName(head->nextReservation, fiscalCode);
}

void printSingleReservation(PtrReservation head) {
    if (head != NULL) {
        printf("\tIdentificativo della prenotazione: %d\n", head->reservationId);
        printf("\tCodice fiscale del paziente: %s\n", head->fiscalCodeCustomer);
        switch (head->timeOfTheDay) {
        case 1:
            printf("\tMomento dell'appuntamento: mattina\n");
            break;
        case 2:
            printf("\tMomento dell'appuntamento: pomeriggio\n");
            break;
        case 3:
            printf("\tMomento dell'appuntamento: sera\n");
            break;
        default:
            break;
        }
    }
}

Reservation *loadReservationsFromFile(char *filename) {
    FILE *file;
    Reservation *reservations = NULL;
    file = fopen(filename, "r");
    int fscanfResult = 0;

    //FIXED: ora se non esiste il file non crasha
    if (file == NULL) {
        return NULL;
    }

    if (fgetc(file) == EOF) {
        return NULL;
    }

    fseek(file, 0, SEEK_SET);

    do {
        int reservationId;
        char fiscalCodeCustomer[17];
        int timeOfTheDay;
        fscanfResult = fscanf(file, "%d\t%s\t%d\n", &reservationId, fiscalCodeCustomer, &timeOfTheDay);
        if (fscanfResult != EOF) {
            Reservation *r = createReservation(fiscalCodeCustomer, timeOfTheDay);
            r->reservationId = reservationId;
            reservations = insertReservationOnEnd(reservations, r);
        }
    } while(fscanfResult != EOF);

    return reservations;
}

void printMergedListsOnFileWrapper() {
    Reservation *symptReservations = NULL;
    Reservation *asymptReservations = NULL;
    symptReservations = loadReservationsFromFile("symptomatics.txt");
    asymptReservations = loadReservationsFromFile("asymptomatics.txt");
    Reservation *merged = mergeReservationLists(symptReservations, asymptReservations);
    saveReservationOnFile(merged, "reservations.txt", "w");
}
