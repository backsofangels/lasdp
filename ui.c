#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "reservation.h"
#include "seclib.h"
#include "appointments.h"
#include "covidtest.h"
#include "utils.h"

void mainUiFlow() {
    //  Variable declarations used in the UI flow
    int programExitFlag = 0;
    int branchBetweenPatientOrLab = 0;
    int isUserLoggedIn = -1;
    int userInputStore = 0;

    //  Variabili specifiche per il flusso utente
    int hasUserReserved = 0;
    int appointmentRemovalId = 0;
    int appointmentsDispatched = 0;
    int testsHaveBeenProcessed = 0;


    //  Lo uso come token di sessione per memorizzare il codice fiscale, tipo un JWT ma più basilare
    char sessionUserCode[17];

    // Variables useful on a functional level (I/O of functions)
    PtrReservation symptomatics = NULL;
    PtrReservation asymptomatics = NULL;
    PtrTest covidTests = NULL;
    Day day = initializeDay();

    initDatabase();

    while (programExitFlag == 0) {
        //  Local variables only for data feeding purposes
        int covidTestIdentifierForSearch = 0;

        //  Brancha nel flow giusto
        if (branchBetweenPatientOrLab == 0) {
            clearScreen();
            printf("Ciao, scegli il tuo ruolo\n");
            printf("\t1. Paziente\n\t2. Operatore di laboratorio\n\t3. Uscita dal programma.\n");
            branchBetweenPatientOrLab = sanitizeUserInput(4);
        }

        //  Branch qui
        switch (branchBetweenPatientOrLab)
        {
            case 1:
                //  Flusso specifico paziente
                if (userInputStore == 0) {
                    clearScreen();
                    printf("Effettua la tua scelta\n");
                    printf("1. Registrazione\n");
                    printf("2. Login\n");
                    if (isUserLoggedIn == 0) {
                        printf("3. Effettua una prenotazione\n");
                        printf("4. Mostra le tue prenotazioni\n");
                        printf("5. Mostra i tuoi appuntamenti\n");
                        printf("6. Cancella un appuntamento fissato\n");
                        printf("7. Visualizza i risultati del tampone\n");
                    }
                    printf("8. Esci\n");
                    userInputStore = sanitizeUserInput(9);
                }

                switch (userInputStore)
                {
                    case 1:
                        //  Registrazione
                        if (isUserLoggedIn == 0) {
                            printf("Hai gia' effettuato l'accesso, esci e poi registrati\n");
                            waitInputPrint("Premi ENTER per tornare al menu'.");
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        signUp();
                        userInputStore = 0;
                        break;

                    case 2:
                        //  Login
                        if (isUserLoggedIn != 0) {
                            clearScreen();
                            //  Imposto il token di sessione
                            isUserLoggedIn = loginCustomer(sessionUserCode);
                        } else {
                            printf("Gia' hai effettuato l'accesso, per entrare con una diversa utenza premi prima su \"8. Esci\"\n");
                            waitInputPrint("Premi ENTER per tornare al menu'...");
                        }
                        userInputStore = 0;
                        break;

                    case 3:
                        //  Prenotazione, assumo che l'utente deve essere loggato e non ha prenotato
                        if (isUserLoggedIn == -1) {
                            userInputStore = 0;
                            break;
                        }
                        //  Check se ha già prenotato
                        if (hasUserReserved == 1 || checkReservationAlreadyPerformed(sessionUserCode) == 1) {
                            clearScreen();
                            printf("Puoi effettuare una sola prenotazione al giorno.\n");
                            waitInputPrint("Premi ENTER per tornare al menu'");
                            userInputStore = 0;
                            break;
                        }
                        //  Se loggato, unwinding dell'IO della prenotazione
                        if (isUserLoggedIn == 0) {
                            int isUserSyntomatic = 0;
                            clearScreen();
                            printf("Domanda principale, hai sintomi? Inserisci 1 se li hai, 2 se non li hai\n");
                            printf("Ricorda che i sintomi piu' comuni includono:\n");
                            printf("\t- Tosse\n\t- Temperatura superiore a 37.5\n\t- Perdita del gusto e dell'olfatto\n");
                            printf("\t- Dolori muscolari\n\nSe non sei sicuro, contatta il tuo medico di famiglia!\n");
                            isUserSyntomatic = sanitizeUserInput(3);

                            if (isUserSyntomatic == 1) {
                                symptomatics = performReservation(symptomatics, sessionUserCode);
                                hasUserReserved = 1;
                            } else if (isUserSyntomatic == 2) {
                                asymptomatics = performReservation(asymptomatics, sessionUserCode);
                                hasUserReserved = 1;
                            } else {
                                printf("Inserisci il valore corretto per favore e presta bene attenzione ai sintomi\n");
                            }
                            waitInputPrint("Ricorda il numero della prenotazione, ti servira' in seguito!\nPremi ENTER per tornare al menu'...");
                        }
                        userInputStore = 0;
                        break;

                    case 4:
                        //Mostra prenotazioni
                        if (isUserLoggedIn == -1) {
                            userInputStore = 0;
                            break;
                        }
                        if (hasUserReserved == 1) {
                            clearScreen();
                            printf("Queste sono le tue prenotazioni odierne\n");
                            printReservationByName(symptomatics, sessionUserCode);
                            printReservationByName(asymptomatics, sessionUserCode);
                            waitInputPrint("\n\nPremi ENTER per tornare al menu'");
                        } else {
                            clearScreen();
                            waitInputPrint("Ancora non hai effettuato prenotazioni per oggi.\nPremi ENTER per tornare al menu'.");

                        }
                        userInputStore = 0;
                        break;

                    case 5:
                        //Mostra appuntamenti
                        if (isUserLoggedIn == -1) {
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        printf("\n\nQuesti sono i tuoi apputamenti odierni\nSe non ne vedi, il centro potrebbe ancora non averli confermati\n\n");
                        printAppointmentByFiscalCode(day, sessionUserCode);
                        waitInputPrint("\n\nPremi ENTER per tornare al menu'...");
                        userInputStore = 0;
                        break;

                    case 6:
                        //  Cancellazoine, chiama anche la stampa per chiarezza
                        if (isUserLoggedIn == -1) {
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        printf("\nQuesti sono i tuoi apputamenti odierni\nSe non ne vedi, il centro potrebbe ancora non averli confermati\n\n");
                        printAppointmentByFiscalCode(day, sessionUserCode);
                        if (isUserLoggedIn == 0) {
                            printf("\nInserisci l'identificativo dell'appuntamento\nche vuoi cancellare.\n");
                            appointmentRemovalId = sanitizeUserInput(200);
                            printf("\n");
                        }
                        day = removeAppointmentById(day, appointmentRemovalId);
                        waitInputPrint("Premi ENTER per tornare al menu'");
                        userInputStore = 0;
                        break;

                    case 7:
                        if (isUserLoggedIn == -1) {
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        //  Visualizza risultato tampone
                        printf("Inserisci l'identificativo della prenotazione\n");
                        covidTestIdentifierForSearch = sanitizeUserInput(200);
                        searchAndPrintTestById(covidTestIdentifierForSearch);
                        waitInputPrint("Premi ENTER per tornare al menu'...");
                        userInputStore = 0;
                        break;

                    case 8:
                        //  Uscita e ritorno al menù di scelta iniziale
                        if (symptomatics != NULL) {
                            saveReservationOnFile(symptomatics, "symptomatics.txt", "a");
                        }
                        if (asymptomatics != NULL) {
                            saveReservationOnFile(asymptomatics, "asymptomatics.txt", "a");
                        }
                        clearScreen();
                        //  Resetto tutte le variabili in caso di immediato ritorno nel flow
                        userInputStore = 0;
                        branchBetweenPatientOrLab = 0;
                        isUserLoggedIn = -1;
                        hasUserReserved = 0;
                        memset(sessionUserCode, ' ', 16);
                        //  Svuoto le liste altrimenti si tiene in pancia anche gli appuntamenti precedenti di TUTTI
                        symptomatics = emptyReservationList(symptomatics);
                        asymptomatics = emptyReservationList(asymptomatics);
                        waitInputPrint("Arrivederci\nPremi ENTER per tornare al menu' principale...");
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
                    clearScreen();
                    printf("Effettua la tua scelta\n");
                    printf("1. Registrazione\n");
                    printf("2. Login\n");
                    if (isUserLoggedIn == 0) {
                        printf("3. Programma appuntamenti del giorno\n");
                        printf("4. Aggiungi manualmente appuntamento\n");
                        printf("5. Rimuovi manualmente appuntamento\n");
                        printf("6. Mostra gli appuntamenti del giorno\n");
                        printf("7. Effettua i tamponi\n");
                        printf("8. Visualizza storico dei tamponi\n");
                    }
                    printf("9. Esci\n");
                    userInputStore = sanitizeUserInput(10);
                }

                switch (userInputStore) {
                    case 1:
                        //  Registrazione
                        if (isUserLoggedIn == 0) {
                            waitInputPrint("Non puoi registrarti se sei loggato\nPremi ENTER per tornare al menu'");
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        signUpTestCenter();
                        userInputStore = 0;
                        break;

                    case 2:
                        //  Login
                        if (isUserLoggedIn == 0) {
                            waitInputPrint("Hai gia' effettuato l'accesso\nPremi ENTER per tornare al menu'");
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        isUserLoggedIn = loginTestCenter();
                        userInputStore = 0;
                        break;

                    case 3:
                        //  Dispone gli appuntamenti automagicamente
                        if (isUserLoggedIn == -1) {
                            userInputStore = 0;
                            break;
                        }
                        if (appointmentsDispatched == 1) {
                            clearScreen();
                            waitInputPrint("Gli appuntamenti per il giorno gia' sono stati programmati\nPremi ENTER per tornare al menu'");
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        printMergedListsOnFileWrapper();
                        day = disponiAppuntamentiNew(&appointmentsDispatched);
                        printDailyAppointmentsWrapper(1, day);
                        emptyReservationListsFiles();
                        if (appointmentsDispatched == 1) {
                            waitInputPrint("I tuoi appuntamenti sono stati programmati per il giorno\nPremi ENTER per tornare al menu' e visualizzarli");
                        } else {
                            waitInputPrint("Non c'erano prenotazioni in coda, riprova piu' tardi\nPremi ENTER per tornare al menu'");
                        }
                        userInputStore = 0;
                        break;

                    case 4:
                        //  Aggiunta manuale appuntamento, viene messo SEMPRE in coda a quelli fatti "online"
                        //  che hanon la precedenza su quelli manuali
                        if (isUserLoggedIn == -1) {
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        day = addAppointmentManually(day);
                        waitInputPrint("Premi ENTER per tornare al menu'");
                        userInputStore = 0;
                        break;

                    case 5:
                        //  Eliminazione appuntamento
                        if (isUserLoggedIn == -1) {
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        printf("Questi sono i tuoi appuntamenti programmati per oggi\n");
                        printDailyAppointmentsWrapper(0, day);
                        printf("Inserisci l'id dell'appuntamento da eliminare\n");
                        appointmentRemovalId = sanitizeUserInput(200);
                        day = removeAppointmentById(day, appointmentRemovalId);
                        waitInputPrint("Premi ENTER per tornare al menu'");
                        userInputStore = 0;
                        break;

                    case 6:
                        //  Mostra appuntamenti
                        if (isUserLoggedIn == -1) {
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        printDailyAppointmentsWrapper(0, day);
                        waitInputPrint("Premi ENTER per tornare al menu'");
                        userInputStore = 0;
                        break;

                    case 7:
                        //  Effettua simulazione tamponi con conseguente stampa in log quarantena e test covid
                        if (isUserLoggedIn == -1) {
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        performCovidTests(day);
                        emptyAppointmentsFile();
                        emptyReservationListsFiles();
                        emptyCommonReservationList();
                        day = initializeDay();
                        testsHaveBeenProcessed = 1;
                        waitInputPrint("I tamponi sono stati effettuati e processati\nPremi ENTER per tornare al menu' e per visualizzare lo storico");
                        userInputStore = 0;
                        break;

                    case 8:
                        //  Storico di TUTTI i tamponi
                        if (isUserLoggedIn == -1) {
                            userInputStore = 0;
                            break;
                        }
                        clearScreen();
                        printf("\nOra verra' mostrato lo storico dei tamponi COVID\n");
                        covidTests = loadTestListFromFile();
                        if (covidTests == NULL) {
                            waitInputPrint("Ancora non ci sono tamponi nello storico\nPremi ENTER per tornare al menu'");
                            userInputStore = 0;
                            break;
                        }
                        printCovidTestsHistoryOnScreen(covidTests);
                        waitInputPrint("\n\nPremi ENTER per tornare al menu'");
                        userInputStore = 0;
                        break;

                    case 9:
                        //  Uscita
                        waitInputPrint("Arrivederci\nPremi ENTER per tornare al menu' principale");
                        isUserLoggedIn = -1;
                        userInputStore = 0;
                        branchBetweenPatientOrLab = 0;
                        break;

                    default:
                        userInputStore = 0;
                        break;
                }
                break;

            case 3:
                //  Uscita da programma, distruggo tutto
                if (testsHaveBeenProcessed == 0) {
                    performCovidTests(day);
                    emptyAppointmentsFile();
                    emptyReservationListsFiles();
                    emptyCommonReservationList();
                    day = initializeDay();
                }
                emptyAppointmentsFile();
                emptyReservationListsFiles();
                emptyReservationListsFiles();
                programExitFlag = 1;
                break;

            default:
                branchBetweenPatientOrLab = 0;  //Reset of the branch
                break;
        }
    }
}
