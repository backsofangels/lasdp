#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "covidtest.h"
#include "appointments.h"
#include "quarantinelog.h"

void performCovidTests(Day dailyAppointments) {
    int lastTestingDay = getLastDayOfTestHistory();

    for (int i = 0; i < 3; i++) {
        while (dailyAppointments.timeOfDay[i] != NULL) {
            CovidTest test;
            test.identifier = dailyAppointments.timeOfDay[i]->reservationId;
            strcpy(test.fiscalCode, dailyAppointments.timeOfDay[i]->fiscalCodeCustomer);
            
            if ((rand() % 100) > 40) {
                test.result = 0;
                removeFromQuarantine(test.fiscalCode);
            } else {
                test.result = 1;
                if (isInQuarantine(test.fiscalCode) == 0) {
                    addToQuarantine(test);
                }
            }
            printCovidTestOnFile(test, lastTestingDay);
            dailyAppointments.timeOfDay[i] = dailyAppointments.timeOfDay[i]->nextReservation;
        }
    }
}

void printCovidTestOnFile(CovidTest covidTest, int testingDay) {
    FILE *file = NULL;
    file = fopen("covid-test-history.txt", "a");

    if (file == NULL) {
        return;
    }
    
    fprintf(file, "%d\t%s\t%d\t%d\n", covidTest.identifier, covidTest.fiscalCode, covidTest.result, testingDay+1);
    fclose(file);
}

int getLastDayOfTestHistory() {
    FILE *file = NULL;
    file = fopen("covid-test-history.txt", "r");

    if (file == NULL) {
        return 0;
    }

    fseek(file, -1, SEEK_END);

    int foundLastDay = 0;

    while (foundLastDay != 1) {
        fseek(file, -1, SEEK_CUR);
        char scannedChar = fgetc(file);
        if (scannedChar == '\t') {
            foundLastDay = 1;
        } else {
            fseek(file, -1, SEEK_CUR);
        }
    }

    if (foundLastDay == 1) {
        int lastDay;
        fscanf(file, "%d", &lastDay);
        return lastDay;
    } else {
        return -1;
    }

    fclose(file);
}

PtrTest loadTestListFromFile() {
    FILE *file = NULL;
    file = fopen("covid-test-history.txt", "r");

    if (file == NULL) {
        return NULL;
    }

    PtrTest listHead = NULL;
    int checkFileEnd = 0;

    do {
        PtrTest newNode = createTestNode();
        int identifier;
        int result;
        int testingDay;
        checkFileEnd = fscanf(file, "%d\t%s\t%d\t%d\n", &identifier, newNode->fiscalCode, &result, &testingDay);
        newNode->identifier = identifier;
        newNode->result = result;
        newNode->testingDay = testingDay;
        listHead = insertNodeToEnd(listHead, newNode);
    } while (checkFileEnd != EOF);

    fclose(file);
    
    return listHead;
}

void printCovidTestsHistoryOnScreen(PtrTest head) {
    if (head == NULL) {
        printf("NULL\n");
        return;
    }

    printf("Identificativo del test: %d\n", head->identifier);
    printf("Codice fiscale: %s\n", head->fiscalCode);
    if (head->result == 0) {
        printf("Risultato del test: NEGATIVO\n");
    } else printf("Risultato del test: POSITIVO\n");
    printf("Testato nel giorno N. %d\n\n", head->testingDay);

    printCovidTestsHistoryOnScreen(head->next);
}

PtrTest createTestNode() {
    PtrTest newNode = NULL;
    newNode = (PtrTest) calloc(1, sizeof(NodeTest));
    newNode->next = NULL;
    return newNode;
}

PtrTest insertNodeToEnd(PtrTest head, PtrTest node) {
    if (head == NULL) {
        return node;
    } else {
        head->next = insertNodeToEnd(head->next, node);
    }
    return head;
}

void printCovidHistoryWrapper() {
    PtrTest head = NULL;
    head = loadTestListFromFile();
    printCovidTestsHistoryOnScreen(head);
}

void searchAndPrintTestById(int covidTestId) {
    PtrTest head = NULL;
    head = loadTestListFromFile();

    printTestById(head, covidTestId);
}

void printTestById(PtrTest head, int covidTestId) {
    if (head == NULL) {
        return;
    }

    if (head->identifier == covidTestId) {
        printf("Identificativo del test: %d\n", head->identifier);
        printf("Codice fiscale: %s\n", head->fiscalCode);
        if (head->result == 0) {
            printf("Risultato del test: NEGATIVO\n");
        } else printf("Risultato del test: POSITIVO\n");
        printf("Testato nel giorno N. %d\n\n", head->testingDay);
        return;
    }

    printTestById(head->next, covidTestId);
}