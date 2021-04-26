#ifndef SECLIB_H
#define SECLIB_H

// Messi a 17/21 per considerare anche il carattere di terminazione nullo, in caso di tempo superstite, creare un
// wrapper di fputc ed fgetc per esattamente la lunghezza definita in input del codice o della password o della stringa in generale letta da file
// oppure smette con la terminazione scelta passata anch'essa come parametro.

typedef struct customer {
    char fiscalCode[17];
    char password[21];
} Person;

typedef struct company {
    char identificationNumber[6];
    char password[21];
} TestCenter;

void initCredentialsDb();
void signUp();
void signUpTestCenter();
int checkIfCodeAlreadyPresentInDb(char *codeToCheck, int customerOrTestCenter);
int persistRegistration(Person p);
int persistTestCenterRegistration(TestCenter t);
int loginCustomer(char *sessionFiscalCode);
int loginTestCenter();

#endif
