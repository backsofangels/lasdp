#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "reservation.h"
#include "seclib.h"
#include "appointments.h"
#include "covidtest.h"
#include "utils.h"

void mainUiFlow() {
    // Variable declarations used in the UI flow
    int programExitFlag = 0;
    int branchBetweenPatientOrLab = 0;
    int isUserLoggedIn = -1;
    int userInputStore = 0;

    //Variabili specifiche per il flusso utente
    int hasUserReserved = 0;
    int appointmentRemovalId = 0;


    //Variable useful to store "session" of user
    char sessionUserCode[17];

    // Variables useful on a functional level (I/O of functions)
    PtrReservation symptomatics = NULL;
    PtrReservation asymptomatics = NULL;
    PtrTest covidTests = NULL;
    Day day = initializeDay();

    while (programExitFlag == 0) {
        //Local variables only for data feeding purposes
        int covidTestIdentifierForSearch = 0;

        //Flow different for patient or lab
        if (branchBetweenPatientOrLab == 0) {
            printf("Ciao, scegli il tuo ruolo\n");
            printf("\t1. Paziente\n\t2. Operatore di laboratorio\n\t3. per uscire dal programma.\n");
            scanf("%d", &branchBetweenPatientOrLab);
        }

        //Branching now
        switch (branchBetweenPatientOrLab)
        {
            case 1:
                //Patient flow, unwinding specific branches now
                if (userInputStore == 0) {
                    printf("\nEffettua la tua scelta\n");
                    printf("1. Registrazione\n");
                    printf("2. Login\n");
                    printf("3. Effettua una prenotazione\n");
                    printf("4. Mostra le tue prenotazioni\n");
                    printf("5. Mostra i tuoi appuntamenti\n");
                    printf("6. Cancella un appuntamento\n");
                    printf("7. Visualizza i risultati del tampone\n");
                    printf("8. Esci\n");
                    scanf("%d", &userInputStore);
                }

                switch (userInputStore)
                {
                    case 1:
                        //Registrazione
                        if (isUserLoggedIn != -1) {
                            printf("Hai già effettuato l'accesso, esci e poi registrati\n");
                            break;
                        }
                        signUp();
                        userInputStore = 0;
                        break;
                    
                    case 2:
                        if (isUserLoggedIn != 0) {
                            isUserLoggedIn = loginCustomer(sessionUserCode);
                        } else {
                            printf("Già hai effettuato l'accesso, per entrare con una diversa utenza premi prima su \"8. Esci\"\n");
                        }                        
                        userInputStore = 0;
                        break;

                    case 3:
                        //Prenotazione, assumo che l'utente deve essere loggato e non ha prenotato
                        if (isUserLoggedIn == -1) {
                            printf("Effettua prima l'accesso e poi riprova\n");
                            break;
                        }
                        if (hasUserReserved == 0 && isUserLoggedIn == 0) {
                            int isUserSyntomatic = 0;

                            printf("Domanda principale, hai sintomi? Inserisci 1 se li hai, 2 se non li hai\n");
                            printf("Ricorda che i sintomi più comuni includono:\n");
                            printf("\t- Tosse\n\t- Temperatura superiore a 37.5\n\t- Perdita del gusto e dell'olfatto\n");
                            printf("\t- Dolori muscolari\n\nSe non sei sicuro, contatta il tuo medico di famiglia!\n");
                            scanf("%d", &isUserSyntomatic);

                            if (isUserSyntomatic == 1) {
                                symptomatics = performReservation(symptomatics, sessionUserCode);
                                hasUserReserved = 1;
                            } else if (isUserSyntomatic == 2) {
                                asymptomatics = performReservation(asymptomatics, sessionUserCode);
                                hasUserReserved = 1;
                            } else {
                                printf("Inserisci il valore corretto per favore e presta bene attenzione ai sintomi\n");
                            }
                        } else if (hasUserReserved != 0) {
                            printf("Puoi effettuare una sola prenotazione al giorno.\n");
                        } else if (isUserLoggedIn != 0) {
                            printf("Effettua prima la login per prenotare un tampone\n");
                        }
                        userInputStore = 0;
                        break;

                    case 4:
                        //Mostra prenotazioni
                        //Probabile baco, se esco con un utente e rientro con un altro vedo anche le altre prenotazioni
                        //Da testare
                        if (isUserLoggedIn == -1) {
                            printf("Effettua prima l'accesso e poi riprova\n");
                            break;
                        }
                        printf("Queste sono le tue prenotazioni odierne\n");
                        printReservations(symptomatics, NULL, 0);
                        printReservations(asymptomatics, NULL, 0);
                        userInputStore = 0;
                        break;

                    case 5:
                        //Mostra appuntamenti
                        if (isUserLoggedIn == -1) {
                            printf("Effettua prima l'accesso e poi riprova\n");
                            break;
                        }
                        printf("Questi sono i tuoi apputamenti odierni\n");
                        printAppointmentByFiscalCode(day, sessionUserCode);
                        userInputStore = 0;
                        break;
                    
                    case 6:
                        if (isUserLoggedIn == -1) {
                            printf("Effettua prima l'accesso e poi riprova\n");
                            break;
                        }
                        if (isUserLoggedIn == 0) {
                            printf("Inserisci l'identificativo dell'appuntamento\nche vuoi cancellare.\n");
                            scanf("%d", &appointmentRemovalId);
                        }
                        day = removeAppointmentById(day, appointmentRemovalId);
                        userInputStore = 0;
                        break;

                    case 7:
                        if (isUserLoggedIn == -1) {
                            printf("Effettua prima l'accesso e poi riprova\n");
                            break;
                        }
                        //Visualizza risultato tampone
                        printf("Inserisci l'identificativo della prenotazione\n");
                        scanf("%d", &covidTestIdentifierForSearch);
                        searchAndPrintTestById(covidTestIdentifierForSearch);
                        userInputStore = 0;
                        break;

                    case 8:
                        //Uscita e ritorno al menù di scelta iniziale
                        if (symptomatics != NULL) {
                            saveReservationOnFile(symptomatics, "symptomatics.txt", "a");
                        }
                        if (asymptomatics != NULL) {
                            saveReservationOnFile(asymptomatics, "asymptomatics.txt", "a");
                        }                     
                        userInputStore = 0;
                        branchBetweenPatientOrLab = 0;
                        isUserLoggedIn = -1;
                        hasUserReserved = 0;
                        memset(sessionUserCode, ' ', 16);
                        printf("Arrivederci\n");
                        break;

                    default:
                        //Caso non ammesso, resetto e torno alla scelta
                        userInputStore = 0;
                        break;
                }

                break;

            case 2:
                //Laboratorio
                if (userInputStore == 0) {
                    printf("\nEffettua la tua scelta\n");
                    printf("1. Registrazione\n");
                    printf("2. Login\n");
                    printf("3. Programma appuntamenti del giorno\n");
                    printf("4. Aggiungi manualmente appuntamento\n");
                    printf("5. Rimuovi manualmente appuntamento\n");
                    printf("6. Mostra gli appuntamenti del giorno\n");
                    printf("7. Effettua i tamponi\n");
                    printf("8. Visualizza storico dei tamponi\n");
                    printf("9. Esci\n");
                    scanf("%d", &userInputStore);
                }

                switch (userInputStore) {
                    case 1:
                        if (isUserLoggedIn != -1) {
                            printf("Non puoi registrarti se sei loggato, esci e poi rientra\n");
                            break;
                        }
                        signUpTestCenter();
                        userInputStore = 0;
                        break;
                    
                    case 2:
                        if (isUserLoggedIn != -1) {
                            printf("Hai già effettuato l'accesso\n");
                            break;
                        }
                        isUserLoggedIn = loginTestCenter();
                        userInputStore = 0;
                        break;

                    case 3:
                        if (isUserLoggedIn == -1) {
                            printf("Effettua prima l'accesso e poi riprova\n");
                            break;
                        }
                        printMergedListsOnFileWrapper();
                        day = disponiAppuntamentiNew();
                        printDailyAppointmentsWrapper(1, day);
                        emptyReservationListsFiles();
                        userInputStore = 0;
                        break;

                    case 4:
                        if (isUserLoggedIn == -1) {
                            printf("Effettua prima l'accesso e poi riprova\n");
                            break;
                        }
                        day = addAppointmentManually(day);
                        userInputStore = 0;
                        break;

                    case 5:
                        if (isUserLoggedIn == -1) {
                            printf("Effettua prima l'accesso e poi riprova\n");
                            break;
                        }
                        printf("Inserisci l'id dell'appuntamento da eliminare\n");
                        scanf("%d", &appointmentRemovalId);
                        day = removeAppointmentById(day, appointmentRemovalId);
                        userInputStore = 0;
                        break;

                    case 6:
                        if (isUserLoggedIn == -1) {
                            printf("Effettua prima l'accesso e poi riprova\n");
                            break;
                        }
                        printDailyAppointmentsWrapper(0, day);
                        userInputStore = 0;
                        break;

                    case 7:
                        if (isUserLoggedIn == -1) {
                            printf("Effettua prima l'accesso e poi riprova\n");
                            break;
                        }
                        performCovidTests(day);
                        emptyAppointmentsFile();
                        emptyReservationListsFiles();
                        emptyCommonReservationList();
                        userInputStore = 0;
                        break;

                    case 8:
                        if (isUserLoggedIn == -1) {
                            printf("Effettua prima l'accesso e poi riprova\n");
                            break;
                        }
                        covidTests = loadTestListFromFile();
                        printCovidTestsHistoryOnScreen(covidTests);
                        userInputStore = 0;
                        break;

                    case 9:
                        printf("Arrivederci\n");
                        userInputStore = 0;
                        branchBetweenPatientOrLab = 0;
                        break;

                    default:
                        userInputStore = 0;
                        break;
                }
                break;
            
            case 3:
                emptyAppointmentsFile();
                emptyReservationListsFiles();
                emptyReservationListsFiles();
                programExitFlag = 1;
                break;

            default:
                printf("Caso non esistente, potresti aver sbagliato a digitare\n");
                branchBetweenPatientOrLab = 0;  //Reset of the branch
                break;
        }
    }
}