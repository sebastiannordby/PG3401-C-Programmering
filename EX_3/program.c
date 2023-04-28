#include "include/flight-booking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Menu choices
const unsigned short MN_ADD_FLIGHT = 1;
const unsigned short MN_ADD_PASSENGER = 2;
const unsigned short MN_PRINT_FLIGHT = 3;
const unsigned short MN_FIND_FLIGHT_BY_TIME = 4;
const unsigned short MN_FIND_FLIGHT_BY_INDEX = 5;
const unsigned short MN_DELETE_FLIGHT = 6;
const unsigned short MN_REMOVE_PASSENGER_RES = 7;
const unsigned short MN_CHANGE_PASSENGER_SEAT = 8;
const unsigned short MN_SEARCH_PASSENGER = 9;
const unsigned short MN_PRINT_ALL_FLIGHTS = 10;
const unsigned short MN_EXIT_PROGRAM = 11;

// Declare functions to skip precendence in code
int readMenuChoice(void);
void printMenu(void);
void printMenuHeader(char *header);
void promptAddFlight(FlightList *flightList);
void promptAddPassenger(FlightList *flightList);
void promptPrintFlight(FlightList *flightList);
void promptFindFlightByDepartureTime(FlightList *flightList);
void promptFindFlightByIndex(FlightList *flightList);
void promptDeleteFlight(FlightList *flightList);
void promptRemovePassengerRes(FlightList *flightList);
void promptChangePassengerSeat(FlightList *flightList);
void promptSearchPassenger(FlightList *flightList);
void freeFlightList(FlightList *flightList);
void readString(char *str, int maxLength);
void usePredefinedData(FlightList *flightList);
void printFailedInput(char *inputName);

int main(int argc, char *argv[]) {
    FlightList *flightList = malloc(sizeof(FlightList));
    if(flightList == NULL) {
        printf("Could not create flight list.\r\n");
        return 1;
    }

    // If there are any arguments run with preloaded
    // flight data. Easier for debugging purposes.
    if(argc > 0) {
        usePredefinedData(flightList);
    }

    int choice;

    while((choice = readMenuChoice()) != MN_EXIT_PROGRAM && choice != EOF) {
        switch(choice) {
            case MN_ADD_FLIGHT:
                promptAddFlight(flightList);
                break;
            case MN_ADD_PASSENGER:
                promptAddPassenger(flightList);
                break;
            case MN_PRINT_FLIGHT:
                promptPrintFlight(flightList);
                break;
            case MN_FIND_FLIGHT_BY_TIME:
                promptFindFlightByDepartureTime(flightList);
                break;
            case MN_FIND_FLIGHT_BY_INDEX:
                promptFindFlightByIndex(flightList);
                break;
            case MN_DELETE_FLIGHT:
                promptDeleteFlight(flightList);
                break;
            case MN_REMOVE_PASSENGER_RES:
                promptRemovePassengerRes(flightList);
                break;
            case MN_CHANGE_PASSENGER_SEAT:
                promptChangePassengerSeat(flightList);
                break;

            case MN_PRINT_ALL_FLIGHTS:
                printFlightList(flightList);
                break;
        }
    }

    freeFlightList(flightList);
    printf("\r\nProgram exited. You can now close the terminal.\r\n");

    return 0;
}

void printMenu(void) {
    printf("----------- Menu -----------\r\n");
    printf("%d. Add flight\r\n", MN_ADD_FLIGHT);
    printf("%d. Add passenger to a flight\r\n", MN_ADD_PASSENGER);
    printf("%d. Print flight\r\n", MN_PRINT_FLIGHT);
    printf("%d. Find flight by time\r\n", MN_FIND_FLIGHT_BY_TIME);
    printf("%d. Find flight by index\r\n", MN_FIND_FLIGHT_BY_INDEX);
    printf("%d. Delete a flight\r\n", MN_DELETE_FLIGHT);
    printf("%d. Remove passenger reservation\r\n", MN_REMOVE_PASSENGER_RES);
    printf("%d. Change seat for passenger\r\n", MN_CHANGE_PASSENGER_SEAT);
    printf("%d. Search for passenger\r\n", MN_SEARCH_PASSENGER);
    printf("%d. List all flights\r\n", MN_PRINT_ALL_FLIGHTS);
    printf("%d. Exit the program\r\n", MN_EXIT_PROGRAM);
}

int readMenuChoice() {
    printMenu();
    printf("booking> ");
    int choice;
    int scanFRes = scanf("%d", &choice);
    getchar();

    return scanFRes != EOF ? choice : EOF;
}

void printMenuHeader(char *text) {
    printf("-------------------- %s --------------------\r\n", text);
}

void readString(char *str, int maxLength) {
    int current, counter = 0;

    while(counter < maxLength && (current = getchar()) != '\n') {
        str[counter] = current;
        counter++;
    }

    str[counter] = '\0';
}

void promptAddFlight(FlightList *flightList) {
    printMenuHeader("ADD FLIGHT");

    char flightId[FLIGHT_ID_MAX_LENGTH];
    printf("Enter id: ");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    char destination[FLIGHT_DESTINATION_MAX_LENGTH];
    printf("Enter destination: ");
    readString(destination, FLIGHT_DESTINATION_MAX_LENGTH);

    unsigned short numberOfSeats;
    printf("Enter number of seats: ");
    if(scanf("%hu", &numberOfSeats) != 1) {
        printFailedInput("number of seats");
        return;
    }

    unsigned short departureTime;
    printf("Enter departure time: ");
    if(scanf("%hu", &departureTime) != 1) {
        printFailedInput("departure time");
        return;
    }

    Flight *flight = addFlight(flightList, flightId, destination, numberOfSeats, departureTime);

    if(flight != NULL) {
        printf("Successfully created flight \"%s\"\r\n", flightId);
        printf("Here is the updated flight list:\r\n");
        printFlightList(flightList);
        printf("\r\n");
    }
}

void promptAddPassenger(FlightList *flightList) {
    printMenuHeader("ADD PASSENGER TO A FLIGHT");

    char flightId[FLIGHT_ID_MAX_LENGTH];
    printf("Enter flight id: ");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    Flight* flight = getFlightById(flightList, flightId);
    if(flight == NULL) {
        printf("No flight with id of: %s\r\n", flightId);
        return;
    }

    char name[PASSENGER_NAME_MAX_LENGTH];
    printf("Enter name: ");
    readString(name, PASSENGER_NAME_MAX_LENGTH);

    unsigned short age;
    printf("Enter age: ");
    if(scanf("%hu", &age) != 1) {
        printFailedInput("age");
        return;
    }

    unsigned short seatNumber;
    printf("Enter seat number(-1 for automatic allocation): ");
    if(scanf("%hu", &seatNumber) != 1) {
        printFailedInput("seatnumber");
        return;
    }

    if(addPassenger(flight, seatNumber, name, age)) {
        printf("Successfully added passenger \"%s\" to flight \"%s\"\r\n", 
            flightId, name);
    } else {
        printf("Could not add passenger to flight.\r\n");
    }
}

void freeFlightList(FlightList *flightList) {
    Flight *pFlight = flightList->head;

    while (pFlight != NULL) {
        Passenger *pPassenger = pFlight->pPassengers;

        while (pPassenger != NULL) {
            Passenger *pPassengerTemp = pPassenger;
            pPassenger = pPassenger->pNext;
            free(pPassengerTemp);
        }
    
        Flight *pTempFlight = pFlight;
        pFlight = pFlight->pNext;
        free(pTempFlight);
    }

    free(flightList);
    flightList = NULL;
}

void promptPrintFlight(FlightList *flightList) {
    printMenuHeader("PRINT FLIGHT");
    char flightId[FLIGHT_ID_MAX_LENGTH];
    printf("Enter flight id: ");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    Flight* flight = getFlightById(flightList, flightId);
    if(flight == NULL) {
        printf("No flight with id of: %s\r\n", flightId);
        return;
    }

    printf("Information for flight %s\r\n", flightId);
    printFlight(flight);
}

void promptFindFlightByIndex(FlightList *flightList) {
    printMenuHeader("FIND FLIGHT BY INDEX");
    printf("Enter index: ");
    unsigned short index;
    if(scanf("%hu", &index) != 1) {
        printFailedInput("index");
        return;
    }

    Flight *flight = getFlightByIndex(flightList, index);

    if(flight == NULL) 
        return;

    printFlight(flight);
}

void promptFindFlightByDepartureTime(FlightList *flightList) {
    printMenuHeader("FIND FLIGHT BY DEPARTURE TIME");
    printf("Enter departure: ");
    unsigned short departureTime;
    if(scanf("%hu", &departureTime) != 1) {
        printFailedInput("departure time");
        return;
    }

    Flight *flight = getFlightByDepartureTime(flightList, departureTime);

    if(flight == NULL) {
        printf("Could not find flight with depature at \"%hu\"\r\n", departureTime);
        return;
    }

    printf("Found flight with depature at \"%hu\"\r\n", departureTime);
    printFlight(flight);
}

void promptDeleteFlight(FlightList *pFlightList) {
    printMenuHeader("DELETE FLIGHT");
    char flightId[FLIGHT_ID_MAX_LENGTH];
    printf("Enter flight id: ");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    if(deleteFlight(pFlightList, flightId)) 
        printf("Successfully deleted flight \"%s\"\r\n", flightId);
    else 
        printf("Failed to delete flight with id of: \"%s\"\r\n", flightId);
}

void promptRemovePassengerRes(FlightList *flightList) {
    printMenuHeader("REMOVE PASSENGER RESERVATION");

    char flightId[FLIGHT_ID_MAX_LENGTH];
    printf("Enter flight id: ");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    unsigned short seatNumber;
    printf("Enter seat number: ");
    if(scanf("%hu", &seatNumber) != 1) {
        printFailedInput("seatnumber");
        return;
    }

    if(removePassenger(flightList, flightId, seatNumber)) 
        printf("Successfully removed passenger reservation.\r\n");
    else 
        printf("Failed to delete passenger reservation.\r\n");
}

void printFailedInput(char *inputName) {
    printf("An error occured inputing %s\r\n", inputName);
}

void promptChangePassengerSeat(FlightList *pFlightList) {
    printMenuHeader("CHANGE PASSENGER SEAT");

    char flightId[FLIGHT_ID_MAX_LENGTH];
    printf("Enter flight id: ");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    printf("Enter current seat number: ");
    unsigned short currentSeatNumber;
    if(scanf("%hu", &currentSeatNumber) != 1) {
        printFailedInput("current seat number");
        return;
    }

    printf("Enter requested seat number: ");
    unsigned short requestedSeatNumber;
    if(scanf("%hu", &requestedSeatNumber) != 1) {
        printFailedInput("current seat number");
        return;
    }

    if(changePassengerSeat(pFlightList, flightId, currentSeatNumber, requestedSeatNumber)) 
        printf("Successfully changed seat from \"%hu\" to \"%hu\"\r\n",
            currentSeatNumber, requestedSeatNumber);
    else 
        printf("Unable to change seat from \"%hu\" to \"%hu\"\r\n", 
            currentSeatNumber, requestedSeatNumber);
}

void usePredefinedData(FlightList *pFlightList) {
    printf("Using predefined data..\r\n");

    printf("Adding flights...\r\n");

    char f1Id[FLIGHT_ID_MAX_LENGTH] = "OSL1234\0";
    char f1Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Oslo Lufthavn\0";
    char f2Id[FLIGHT_ID_MAX_LENGTH] = "GDN1234\0";
    char f2Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Gdansk Airport\0";
    char f3Id[FLIGHT_ID_MAX_LENGTH] = "AYA1234\0";
    char f3Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Alanya International\0";
    char f4Id[FLIGHT_ID_MAX_LENGTH] = "CHN1234\0";
    char f4Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Copenhagen\0";
    char f5Id[FLIGHT_ID_MAX_LENGTH] = "STC1234\0";
    char f5Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Stockholm Airport\0";
    char f6Id[FLIGHT_ID_MAX_LENGTH] = "TEN1234\0";
    char f6Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Tenerife Intr Airport\0";

    Flight* f1 = addFlight(pFlightList, f1Id, f1Dest, 22, 820);
    Flight* f2 = addFlight(pFlightList, f2Id, f2Dest, 22, 1020);
    Flight* f3 = addFlight(pFlightList, f3Id, f3Dest, 22, 1120);
    Flight* f4 = addFlight(pFlightList, f4Id, f4Dest, 22, 1220);
    Flight* f5 = addFlight(pFlightList, f5Id, f5Dest, 22, 1320);
    Flight* f6 = addFlight(pFlightList, f6Id, f6Dest, 22, 1420);

    printf("Adding passengers\r\n");

    char f1P1Name[PASSENGER_NAME_MAX_LENGTH] = "Dennis MacAlistair Ritchie";
    char f1P2Name[PASSENGER_NAME_MAX_LENGTH] = "Aaron Swartz";
    char f1P3Name[PASSENGER_NAME_MAX_LENGTH] = "Linus Thorvalds";
    addPassenger(f1, 0, f1P1Name, 55);
    addPassenger(f1, 0, f1P2Name, 23);
    addPassenger(f1, 0, f1P3Name, 56);

    char f2P1Name[PASSENGER_NAME_MAX_LENGTH] = "Alan Turing";
    char f2P2Name[PASSENGER_NAME_MAX_LENGTH] = "Bill Gates";
    char f2P3Name[PASSENGER_NAME_MAX_LENGTH] = "Linus Thorvalds";
    addPassenger(f2, 0, f2P1Name, 21);
    addPassenger(f2, 0, f2P2Name, 42);
    addPassenger(f2, 0, f2P3Name, 65);

    char f6P1Name[PASSENGER_NAME_MAX_LENGTH] = "Tim Berners-Lee";
    char f6P2Name[PASSENGER_NAME_MAX_LENGTH] = "James Gosling";
    char f6P3Name[PASSENGER_NAME_MAX_LENGTH] = "Brian Kernighan";
    addPassenger(f6, 1, f6P1Name, 21);
    addPassenger(f6, 2, f6P2Name, 42);
    addPassenger(f6, 3, f6P3Name, 88);

    printFlight(f6);
    printFlight(f6);

    printf("Program now uses predefined data.\r\n");
}
