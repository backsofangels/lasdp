#ifndef COVIDTEST_H
#define COVIDTEST_H
#include "appointments.h"

typedef struct CovidTest {
    char fiscalCode[17];
    int result;
} CovidTest;

/*
 *  Come mi scrivo i dati nello storico?
 *  
 *  In primis, questi avranno una stesura del tipo CF...\tRISULTATO\tNUMERO_GIORNO
 * 
 *  Il problema è come me li vado a leggere visto che potrei avere numeri di giorno di più caratteri quindi
 *  
 *  Faccio FSEEK alla fine del file, faccio FGETC fin quando non trovo \t. Quando trovo \t significa che ho trovato lo \t prima dell'ultimo
 *  giorno memorizzando, di conseguenza quindi è molto semplice ora affrontare la cosa, basta fare fscanf(%d) e leggo in blocco l'ultimo giorno
 */

void performCovidTests(Day dailyAppointments);
void printCovidTestsOnFile(CovidTest covidTest, int testingDay);
int getLastDayOfTestHistory();


#endif