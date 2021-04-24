#ifndef QUARANTINELOG_H
#define QUARANTINELOG_H
#include "covidtest.h"

void addToQuarantine(CovidTest test);
int isInQuarantine(char *fiscalCode);
void removeFromQuarantine(char *fiscalCode);

#endif