#ifndef SECLIB_H
#define SECLIB_H

// Astrae il paziente, fiscalCode è a 17 per considerare stringa di lunghezza 16 più il carattere di terminazione.
// Stesso ragionamento si applica a password e ad identificationNumber, che rappresenta la matricola di un operatore sanitario.
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
