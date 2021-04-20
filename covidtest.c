#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "covidtest.h"
#include "appointments.h"

void performCovidTests(Day dailyAppointments) {
    int lastTestingDay = getLastDayOfTestHistory();

    if (lastTestingDay == -1) {
        return;
    }

    for (int i = 0; i < 3; i++) {
        CovidTest test;
        strcpy(test.fiscalCode, dailyAppointments.timeOfDay[i]->fiscalCodeCustomer);
        if ((rand() % 100) > 10) {
            test.result = 0;
        } else test.result = 1;
        printCovidTestOnFile(test, lastTestingDay);
    }
}

void printCovidTestOnFile(CovidTest covidTest, int testingDay) {
    FILE *file = NULL;
    file = fopen("covid-test-history.txt", "a");

    if (file == NULL) {
        return;
    }
    
    fprintf(file, "%s\t%d\t%d", covidTest.fiscalCode, covidTest.result, testingDay+1);
}

int getLastDayOfTestHistory() {
    FILE *file = NULL;
    file = fopen("covid-test-history.txt", "r");

    if (file == NULL) {
        return -1;
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
}