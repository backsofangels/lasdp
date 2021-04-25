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
        printf("Spiacente, c'è stato un errore nella registrazione, riprova.\n");
    }
}

void signUpTestCenter() {
    TestCenter t;
    int registrationOutcome;
    printf("Inserisci il tuo codice operatore.\n");
    scanf("%5s", t.identificationNumber);
    printf("Inserisci la tua password.\n");
    scanf("%20s", t.password);
    registrationOutcome = persistTestCenterRegistration(t);
    if (registrationOutcome == 0) {
        printf("Ti sei registrato con successo!\n");
    } else {
        printf("Spiacente, c'è stato un errore nella registrazione, riprova.\n");
    }
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
}

int loginTestCenter() {
    FILE *testCenterOperatorDatabase = NULL;
    testCenterOperatorDatabase = fopen("testcenterregistration.txt", "r");

    if (testCenterOperatorDatabase == NULL) {
        return -1;
    }

    char userInputOperatorId[6];
    char userInputPassword[21];

    printf("Inserisci il tuo codice operatore\n");
    scanf("%5s", userInputOperatorId);
    printf("Inserisci la tua password\n");
    scanf("%20s", userInputPassword);

    int eofCheck = 0;
    int codeCompare = 1;

    do {
        char fileOpCode[6];
        char filePassword[21];

        eofCheck = fscanf(testCenterOperatorDatabase, "%s\t%s\n", fileOpCode, filePassword);
        codeCompare = strcasecmp(fileOpCode, userInputOperatorId);

        if (codeCompare == 0) {
            if (strcmp(userInputPassword, fileOpCode) == 0) {
                printf("Login effettuato con successo\n");
                return 0;
            } else {
                printf("Credenziali errate\n");
                return 1;
            }
        }
    } while (eofCheck != EOF && codeCompare != 0);

    printf("Molto probabilmente non sei registrato\n");
    return 2;
}
