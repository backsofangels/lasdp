#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"seclib.h"

//KNOWN ISSUE: Se inserisco un codice o password > 16/20 mi sfancula l'input
void signUp() {
    Person p;
    int registrationOutcome;

    printf("Ora effettuerai la registrazione, segui le istruzioni passo passo\n");
    printf("Inserisci il tuo codice fiscale\n");
    scanf("%16s", p.fiscalCode); //16s to trim the lenght of the input string to the one accepted by the fiscalCode string
    fflush(stdin);
    printf("Insert your password\n");
    scanf("%20s", p.password);


    registrationOutcome = persistRegistration(p);
    if (registrationOutcome == 0) {
        printf("Ti sei registrato con successo!\n");
    } else {
        printf("Spiacenti, c'è stato un errore nella registrazione, riprovare\n");
    }
}

void signUpTestCenter() {
    TestCenter t;
    int registrationOutcome;
    printf("Insert your identificative number\n");
    scanf("%5s", t.identificationNumber);
    printf("Insert your password\n");
    scanf("%20s", t.password);
    registrationOutcome = persistTestCenterRegistration(t);
    if (registrationOutcome == 0) {
        printf("You are now registered\n");
    } else {
        printf("Error in registration\n");
    }
}

void printCredentials(char *fiscalCode, char *password) {
    printf("This is the fiscal code: %s\n", fiscalCode);
    printf("This is the password: %s\n", password);
}

int persistRegistration (Person p) {
    FILE *file;
    file = fopen("customerRegistration.txt", "a+");
    if (file != NULL) {
        fprintf(file, "%s\t%s\n", p.fiscalCode, p.password);
        fflush(stdin);
        fclose(file);
        return 0;
    }
    return -1;
}

int persistTestCenterRegistration (TestCenter t) {
    FILE *file;
    file = fopen("testcenterregistration.txt", "a+");
    if (file != NULL) {
        fprintf(file, "%s\t%s\n", t.identificationNumber, t.password);
        fflush(stderr);
        fclose(file);
        return 0;
    }
    return 1;
}

/*
 *  RETURN CODE POSSIBLE
 *
 *  -1: error opening file
 *  0:  login successful
 *  1:  wrong password
 *  2:  non-existent users
 */


//TODO: ottimizzare questa roba
// 0 = Login con successo
// 1 = Password errata
// 2 = Utente inesistente
// -1 = errore in apertura file
int loginCustomer(char *sessionUserCode) {
    FILE *customerDatabase = NULL;
    customerDatabase = fopen("customerRegistration.txt", "r");

    if (customerDatabase == NULL) {
        //Error in file opening
        return -1;
    }

    //Local input of user datas for login
    char userInputFiscalCode[17];
    char userInputPassword[21];
    printf("Inserisci il tuo codice fiscale\n");
    scanf("%16s", userInputFiscalCode);
    printf("Inserisci la tua password\n");
    scanf("%20s", userInputPassword);

    int eofCheck = 0;
    int fcCompare = 1;

    do {
        char fileFiscalCode[17];
        char filePassword[21];
        eofCheck = fscanf(customerDatabase, "%s\t%s\n", fileFiscalCode, filePassword);
        fcCompare = strcasecmp(fileFiscalCode, userInputFiscalCode);
        if (fcCompare == 0) {
            if (strcmp(userInputPassword, filePassword) == 0) {
                strcpy(sessionUserCode, fileFiscalCode);
                printf("Login effettuato con successo!\n");
                return 0;
            } else return 1;
        }
    } while (fcCompare != 0 && eofCheck != EOF);
    
    //Se arrivo qui significa che non ho mai trovato l'utente e quindi non è registrato
    printf("Molto probabilmente non sei registrato, per favore registrati\n");
    return 2;
    
    // if (file != NULL) {
    //     char localFiscalCode[17];
    //     char localPassword[21];
    //     int eof = 1;
    //     do {

    //         eof = fscanf(file, "%s\t%s\n", localFiscalCode, localPassword);
    //         printf("Fetched fiscal code: %s\n", localFiscalCode);
    //         if (strcasecmp(localFiscalCode, fiscalCode) == 0) {
    //             //fscanf(file, "%s\n", localPassword);
    //             if (strcmp(password, localPassword) == 0) {
    //                 printf("It's a match!\n");
    //                 strcpy(sessionUserCode, fiscalCode);
    //                 return 0;
    //             } else {
    //                 printf("not matching passwords\n");
    //             }
    //         }
    //         printf("eof value: %d\n", eof);
    //     }
    //     while (strcasecmp(localFiscalCode, fiscalCode) != 0 && eof != -1); //controllare valore tornato da eof su windows
    //     return 1;
    // }
    // return 2;
}

int loginTestCenter() {
    char idNUmber[17];
    char password[21]; //put to all 0 after readout
    printf("Please enter your fiscal code\n");
    scanf("%5s", idNUmber);
    printf("Insert your password\n");
    scanf("%20s", password);
    FILE *file;
    file = fopen("testcenterregistration.txt", "r");
    if (file != NULL) {
        char localIdNumber[6];
        char localPassword[21];
        int eof = 1;
        do {

            eof = fscanf(file, "%s\t%s\n", localIdNumber, localPassword);
            printf("Fetched id number: %s\n", localIdNumber);
            if (strcasecmp(localIdNumber, idNUmber) == 0) {
                //fscanf(file, "%s\n", localPassword);
                if (strcmp(password, localPassword) == 0) {
                    printf("It's a match!\n");
                    return 0;
                } else {
                    printf("not matching passwords\n");
                    return 1;
                }
            }
            printf("eof value: %d\n", eof);
        }
        while (strcasecmp(localIdNumber, idNUmber) != 0 && eof != -1); //controllare valore tornato da eof su windows

    }
    return -1;
}
