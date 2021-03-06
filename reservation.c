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

Reservation *deleteReservation(Reservation *r, int reservationId) {
    if (r == NULL) { return r; }
    if (r->reservationId == reservationId) {
        Reservation *tmp = r->nextReservation;
        r->nextReservation = NULL;
        free(r);
        return tmp;
    } else r->nextReservation = deleteReservation(r->nextReservation, reservationId);
    return r;
}


Reservation *mergeReservationLists(Reservation *symptomatics, Reservation *asymptomatics) {
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
        printf("nullFile\n");
        printReservations(headOfReservation, NULL, 0);
    } else if (file != NULL) {
        printReservations(headOfReservation, file, 1);
    }

    fclose(file);
}

Reservation *searchReservationById(Reservation *headOfReservation, int reservationId) {
    printf("searchReservationById()\n");
    if (headOfReservation == NULL) {
        return headOfReservation;
    } else if (headOfReservation->reservationId == reservationId) {
        return headOfReservation;
    }
    headOfReservation = searchReservationById(headOfReservation->nextReservation, reservationId);
}

Reservation *searchReservationByCustomer(Reservation *headOfReservation, char *customerFiscalCode) {
    printf("searchReservationByCustomer()\n");
    if (headOfReservation == NULL) {
        return headOfReservation;
    }
    if (strcmp(headOfReservation->fiscalCodeCustomer, customerFiscalCode) == 0) {
        return headOfReservation;
    }
    headOfReservation = searchReservationByCustomer(headOfReservation->nextReservation, customerFiscalCode);
}

Reservation *searchReservationByTimeOfDay (Reservation *headOfReservation, int timeOfTheDay) {
    printf("searchReservationByTimeOfDay()\n");
    Reservation *localHead = headOfReservation;
    Reservation *results = NULL;

    while (localHead != NULL) {
            printf("LocalHead not null. ");
            if (localHead->timeOfTheDay == timeOfTheDay) {
                Reservation *temp = createReservation(localHead->fiscalCodeCustomer, timeOfTheDay);
                temp->reservationId = localHead->reservationId;
                printf("Found node. ");
                results = insertReservationOnEnd(results, temp);
                printf("Going next.\n");
            } else printf ("Not found node. \n");
        localHead = localHead->nextReservation;
    }

    return results;
}

void printReservations(Reservation *head, FILE *file, int printOnFile) {
    if (head == NULL) {
        return;
    }
    if (file != NULL && printOnFile == 1) {
        fprintf(file, "%d\t%s\t%d\n", head->reservationId, head->fiscalCodeCustomer, head->timeOfTheDay);
    } else if (printOnFile == 0) { 
        printf("%d\t%s\t%d\n", head->reservationId, head->fiscalCodeCustomer, head->timeOfTheDay); 
    }
    printReservations(head->nextReservation, file, printOnFile);
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