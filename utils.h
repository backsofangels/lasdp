#ifndef UTILS_H
#define UTILS_H

void emptyReservationListsFiles();
void emptyCommonReservationList();
void emptyAppointmentsFile();
void initDatabase();
void clearScreen();
void waitInputPrint(char *message);
int sanitizeUserInput(int maxUserInput);

#endif
