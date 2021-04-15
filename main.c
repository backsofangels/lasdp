#include <stdio.h>
#include <stdlib.h>
#include "seclib.h"

int main() {
    int actionFlowControl = 0;
    int terminationFlag = 0;
    int isCustomerOrCompany = 0; //0 is default init, 1 for customer, 2 for company, not admitted other values
    int loginMode = 0;
    int isLoggedIn = 0;

    //TODO: FIX if login mode is a character, it loops
    while (terminationFlag == 0) {
        if (isCustomerOrCompany == 0) {
            printf("Please make your choice.\n Who are you?\n");
            printf("Insert 1 for customer\n");
            printf("Insert 2 for company\n");
            scanf("%d", &isCustomerOrCompany);
        }

        switch (isCustomerOrCompany) {
            case 1:
            printf("Please make your choice\n");
            printf("1. Register\n2. Login\n5. Exit\n");
            scanf("%d", &actionFlowControl);
            switch (actionFlowControl)
            {
                case 1:
                printf("Please register yourself\n");
                signUp();
                break;

                case 2:
                isLoggedIn = login();
                printf("Login result is %d\n", isLoggedIn);
                break;

                case 5:
                terminationFlag = 1;
                printf("Bye.\n");
                break;
            }
            break;

            case 2:
            printf("Please make your choice\n");
            printf("1. Register\n2. Login\n5. Exit\n");
            scanf("%d", &actionFlowControl);
            switch (actionFlowControl)
            {
                case 1:
                signUpTestCenter();
                break;

                case 2:
                isLoggedIn = loginTestCenter();
                printf("Login result is %d\n", isLoggedIn);
                break;

                case 5:
                //Termination
                terminationFlag = 1;
                printf("Bye.\n");
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
