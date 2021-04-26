#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seclib.h"
#include "utils.h"


//  Se non esistono i file di persistenza, li creo
void initCredentialsDb() {
    FILE *testCenter = NULL;
    FILE *customer = NULL;

    customer = fopen("customerRegistration.txt", "r");
    testCenter = fopen("testcenterregistration.txt", "r");

    if (customer == NULL) {
        customer = fopen("customerRegistration.txt", "w");
        fclose(customer);
    }

    if (testCenter == NULL) {
        testCenter = fopen("testcenterregistration.txt", "w");
        fclose(testCenter);
    }
}

//  Signup specifica per l'utente dato che contiene I/O specifico
void signUp() {
    Person p;
    int registrationOutcome;

    printf("Ora effettuerai la registrazione, segui le istruzioni passo passo\n");
    printf("\nInserisci il tuo codice fiscale\n(max. 16 caratteri, eventuali eccessi verranno scartati a partire dal primo carattere)\n");
    scanf("%16s", p.fiscalCode); //formattato a 16 per accomodare solo gli ultimi 16 caratteri inseriti dall'utente a schermo
    fflush(stdin);
    if (checkIfCodeAlreadyPresentInDb(p.fiscalCode, 0) == 1) {  // Check se l'utente è già registrato
        waitInputPrint("\nSei gia' registrato, effettua il login\nPremi un tasto per tornare al menu'");
        return;
    }
    printf("\nInserisci la password\n");
    scanf("%20s", p.password);
    fflush(stdin);
    registrationOutcome = persistRegistration(p);
    if (registrationOutcome == 0) {
        waitInputPrint("\nTi sei registrato con successo!\nPremi un tasto per tornare al menu'.");
    } else {
        waitInputPrint("\nSpiacente, c'è stato un errore nella registrazione, riprova.\nPremi un tasto per tornare al menu'.");
    }
}

void signUpTestCenter() {
    TestCenter t;
    int registrationOutcome;
    printf("Inserisci il tuo codice operatore.\n(Max 5 caratteri, eventuali eccessi verranno scartati a partire dal primo carattere)\n");
    scanf("%5s", t.identificationNumber);
    if (checkIfCodeAlreadyPresentInDb(t.identificationNumber, 1) == 1) {
        waitInputPrint("\nSei gia' registrato, effettua il login\nPremi un tasto per tornare al menu'");
        return;
    }
    fflush(stdin);
    printf("Inserisci la tua password.\n");
    scanf("%20s", t.password);
    fflush(stdin);
    registrationOutcome = persistTestCenterRegistration(t);
    if (registrationOutcome == 0) {
        waitInputPrint("Ti sei registrato con successo!\nPremi un tasto per tornare al menu'.");
    } else {
        waitInputPrint("\nSpiacente, c'è stato un errore nella registrazione, riprova.\nPremi un tasto per tornare al menu'.");
    }
}

// customerOrTestCenter = 0 fa il controllo in customer, 1 in test center
int checkIfCodeAlreadyPresentInDb(char *codeToCheck, int customerOrTestCenter) {
    FILE *database = NULL;
    int isPresent = 0;

    if (customerOrTestCenter == 0) {
        database = fopen("customerRegistration.txt", "r");
    } else if (customerOrTestCenter == 1) {
        database = fopen("testcenterregistration.txt", "r");
    }

    while (feof(database) == 0 && isPresent == 0) {
        char code[17];
        char pass[21];
        fscanf(database, "%s\t%s\n", code, pass);
        if (strcasecmp(code, codeToCheck) == 0) {
            isPresent = 1;
        } else {
        }
    }

    fclose(database);
    return isPresent;
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
    printf("\nInserisci il tuo codice fiscale\n");
    scanf("%16s", userInputFiscalCode);
    fflush(stdin);
    printf("\nInserisci la tua password\n");
    scanf("%20s", userInputPassword);
    fflush(stdin);

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
                waitInputPrint("\nLogin effettuato con successo!\nPremi INVIO per continuare");
                return 0;
            } else {
                waitInputPrint("\n\nCredenziali errate\n\n\tPremi un tasto per tornare al menu'");
                return 1;
            }

        }
    } while (fcCompare != 0 && eofCheck != EOF);

    //Se arrivo qui significa che non ho mai trovato l'utente e quindi non è registrato
    waitInputPrint("\n\nMolto probabilmente non sei registrato\n\n\tPremi un tasto per tornare al menu'");
    return 2;
}

//  Retcode uguali a loginCustomer()
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
    fflush(stdin);
    printf("Inserisci la tua password\n");
    scanf("%20s", userInputPassword);
    fflush(stdin);

    int eofCheck = 0;
    int codeCompare = 1;

    do {
        char fileOpCode[6];
        char filePassword[21];

        eofCheck = fscanf(testCenterOperatorDatabase, "%s\t%s\n", fileOpCode, filePassword);
        codeCompare = strcasecmp(fileOpCode, userInputOperatorId);

        if (codeCompare == 0) {
            if (strcmp(userInputPassword, filePassword) == 0) {
                waitInputPrint("\n\nLogin effettuato con successo\n\n\tPremi un tasto per tornare al menu'");
                return 0;
            } else {
                waitInputPrint("\n\nCredenziali errate\n\n\tPremi un tasto per tornare al menu'");
                return 1;
            }
        }
    } while (eofCheck != EOF && codeCompare != 0);

    waitInputPrint("\n\nMolto probabilmente non sei registrato\n\n\tPremi un tasto per tornare al menu'");
    return 2;
}
