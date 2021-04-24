#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "covidtest.h"

void addToQuarantine(CovidTest test) {
    FILE *file = NULL;
    file = fopen("quarantine-log.txt", "a");

    if (file == NULL) {
        return;
    }

    fprintf(file, "%s\n", test.fiscalCode);

    fclose(file);
}

int isInQuarantine (char *fiscalCode) {
    FILE *file = NULL;
    file = fopen("quarantine-log.txt", "r");

    if (file == NULL) {
        return -1;
    }
    char scanned[16];

    int equals = 1;
    int eof = 0;

    do {
        eof = fscanf(file, "%s\n", scanned);
        equals = strcmp(scanned, fiscalCode);
    } while (equals == 1 && eof != EOF);

    fclose(file);

    return equals;
}

void removeFromQuarantine(char *fiscalCode) {
    FILE *file = NULL;
    file = fopen("quarantine-log.txt", "a+");

    if (file == NULL) {
        return;
    }

    rewind(file);
    char scanned[16];
    int eof = 0;
    int equals = 1;

    do {
        eof = fscanf(file, "%s\n", scanned);
        equals = strcmp(scanned, fiscalCode);
    } while (equals == 1 && eof != EOF);

    if (equals == 0) {
        fseek(file, -16, SEEK_CUR);
        fwrite("", sizeof(char), 16, file);
    }

    fclose(file);
}