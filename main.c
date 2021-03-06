#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "seclib.h"
#include "reservation.h"
#include "appointments.h"
#include "covidtest.h"

int main() {
    srand(time(NULL));
    int actionFlowControl = 0;
    int terminationFlag = 0;
    int isCustomerOrCompany = 0; //0 is default init, 1 for customer, 2 for company, not admitted other values
    //int loginMode = 0;
    char sessionFiscalCode[16];     //Rifare meglio, ho davvero senso di conservarmi il dato qui e indubbiamente, ma posso usarne uno solo
                                    //oppure devo usarne due?
    int isLoggedIn = 0;
    Reservation* symptomatics = NULL;
    Reservation* asymptomatics = NULL;
    int hasReserved = 0;
    //TODO: FIX if login mode is a character, it loops
    while (terminationFlag == 0) {
        int hasSymptoms = 0;

        Day day = initializeDay();

        if (isCustomerOrCompany == 0) {
            printf("Please make your choice.\n Who are you?\n");
            printf("Insert 1 for customer\n");
            printf("Insert 2 for company\n");
            scanf("%d", &isCustomerOrCompany);
        }

        switch (isCustomerOrCompany) {
            case 1:
            printf("Please make your choice\n");
            printf("1. Register\n2. Login\n3. Perform a reservation\n4. Show your reservations\n5. Exit\n6. Delete appointment");
            scanf("%d", &actionFlowControl);
            switch (actionFlowControl) {
                case 1:
                printf("Please register yourself\n");
                signUp();
                break;

                case 2:
                isLoggedIn = login(sessionFiscalCode);
                printf("Login result is %d\n", isLoggedIn);
                break;

                case 3: //Assunzione, l'utente può fare una sola prenotazione
                    if (hasReserved == 0) {
                        printf("Add your reservation now.\nDo you have symptoms?\n1. Yes\n2. No\n");
                        scanf("%d", &hasSymptoms);
                        if (hasSymptoms == 1) {
                            symptomatics = performReservation(symptomatics, sessionFiscalCode);
                        }
                        else if (hasSymptoms == 2) {
                            asymptomatics = performReservation(asymptomatics, sessionFiscalCode);
                        }
                        else {
                            printf("Impossibile\n");
                        }
                        hasReserved = 1;
                    } else printf("You cannot reserve more than 1 time per day\n");
                break;

                case 4:
                    printf("Showing now your reservations\n");
                    printReservations(symptomatics, NULL, 0);
                    printReservations(asymptomatics, NULL, 0);
                break;

                case 5:
                terminationFlag = 1;
                if (symptomatics != NULL) {
                    saveReservationOnFile(symptomatics, "symptomatics.txt", "a");
                }
                if (asymptomatics != NULL) {
                    saveReservationOnFile(asymptomatics, "asymptomatics.txt", "a");
                }
                printf("Bye.\n");
                break;

                case 6:
                    day = disponiAppuntamentiNew();
                    day = removeAppointmentById(day, 100);
                    break;

                case 20:
                    searchAndPrintTestById(107);
                    break;

                case 21:
                    day = disponiAppuntamentiNew();
                    printAppointmentByFiscalCode(day, "salva");
                    break;
                    
                default:
                    break;
            }
            break;

            case 2:
            printf("Please make your choice\n");
            printf("1. Register\n2. Login\n5. Exit\n");
            scanf("%d", &actionFlowControl);
            switch (actionFlowControl) {
                case 1:     //Registration
                    signUpTestCenter();
                    break;

                case 2:     //Login
                    isLoggedIn = loginTestCenter();
                    printf("Login result is %d\n", isLoggedIn);
                    break;

                case 5:     //Termination
                    terminationFlag = 1;
                    printf("Bye.\n");
                    break;

                case 25:    //Visualizza richieste di tamponi e fissa apputamenti
                    printMergedListsOnFileWrapper();
                    day = disponiAppuntamentiNew();
                    printDailyAppointmentsWrapper(1, day);
                    printf("\nBefore\n");
                    printDailyAppointmentsWrapper(0, day);
                    day = removeAppointmentById(day, 145);
                    printf("\nAfter\n");
                    printDailyAppointmentsWrapper(0, day);
                    break;
                
                case 26: //Aggiungi manualmente appuntamento
                    break;
                
                case 27: //Rimuovi manualmente appuntamento
                    break;

                case 28: //Stampa appuntamenti del giorno (anche su file)
                    break;

                case 29:    //Simulazione test covid
                    day = disponiAppuntamentiNew();
                    performCovidTests(day);
                    break;

                case 30:    //Stampa storico test covid
                    loadTestListFromFile();
                    break;

                default:
                    printf("Not admitted\n");
                    break;
            }
            break;

            default:
            printf("Not admitted\n");
            break;
        }
    }

    return 0;
}
