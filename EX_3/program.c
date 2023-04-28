#include "flight-booking.h"
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
            // case MN_REMOVE_PASSENGER_RES:
            //     promptRemovePassengerRes();
            //     break;
            // case MN_CHANGE_PASSENGER_SEAT:
            //     promptChangePassengerSeat();
            //     break;
            // case MN_SEARCH_PASSENGER:
            //     promptSearchPassenger();
            //     break;
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
    printf("---------- %s ----------\r\n", text);
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
    printMenuHeader("Add flight");

    char flightId[FLIGHT_ID_MAX_LENGTH];
    printf("Enter id: ");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    char destination[FLIGHT_DESTINATION_MAX_LENGTH];
    printf("Enter destination: ");
    readString(destination, FLIGHT_DESTINATION_MAX_LENGTH);

    unsigned short numberOfSeats;
    printf("Enter number of seats: ");
    int numberOfSeatsRes = scanf("%hu", &numberOfSeats);

    unsigned short departureTime;
    printf("Enter departure time: ");
    int departureTimeRes = scanf("%hu", &departureTime);

    Flight *flight = addFlight(flightList, flightId, destination, numberOfSeats, departureTime);

    if(flight != NULL) {
        printf("Successfully created flight \"%s\"\r\n", flightId);
        printf("Here is the updated flight list:\r\n");
        printFlightList(flightList);
        printf("\r\n");
    }
}

void promptAddPassenger(FlightList *flightList) {
    printMenuHeader("Add passenger to a flight");

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
    int ageRes = scanf("%hu", &age);

    unsigned short seatNumber;
    printf("Enter seat number(-1 for automatic allocation): ");
    int seatNumberRes = scanf("%hu", &seatNumber);

    if(ageRes == EOF || seatNumberRes == EOF) {
        printf("Could not continue the registation of passenger.");
        return;
    }

    if(addPassenger(flight, seatNumber, name, age)) {
        printf("Successfully added passenger \"%s\" to flight \"%s\"\r\n", 
            flightId, name);
    } else {
        printf("Could not add passenger to flight.");
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

void usePredefinedData(FlightList *flightList) {
    printf("Using predefined data..\r\n");

    printf("Adding flights...\r\n");

    char f1Id[FLIGHT_ID_MAX_LENGTH] = "OSL1234\0";
    char f1Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Oslo Lufthavn\0";
    char f2Id[FLIGHT_ID_MAX_LENGTH] = "GDN1234\0";
    char f2Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Gdansk Airport\0";
    char f3Id[FLIGHT_ID_MAX_LENGTH] = "AYA1234\0";
    char f3Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Alanya International\0";
    char f4Id[FLIGHT_ID_MAX_LENGTH] = "CHN1234\0";
    char f4Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Oslo Lufthavn\0";
    char f5Id[FLIGHT_ID_MAX_LENGTH] = "STC1234\0";
    char f5Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Stockholm Airport\0";
    char f6Id[FLIGHT_ID_MAX_LENGTH] = "TEN1234\0";
    char f6Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Tenerife Intr Airport\0";

    Flight* f1 = addFlight(flightList, f1Id, f1Dest, 22, 820);
    Flight* f2 = addFlight(flightList, f2Id, f2Dest, 22, 1020);
    Flight* f3 = addFlight(flightList, f3Id, f3Dest, 22, 1120);
    Flight* f4 = addFlight(flightList, f4Id, f4Dest, 22, 1220);
    Flight* f5 = addFlight(flightList, f5Id, f5Dest, 22, 1320);
    Flight* f6 = addFlight(flightList, f6Id, f6Dest, 22, 1420);

    printf("Adding passengers\r\n");

    char f1P1Name[PASSENGER_NAME_MAX_LENGTH] = "Dennis MacAlistair Ritchie\0";
    char f1P2Name[PASSENGER_NAME_MAX_LENGTH] = "Aaron Swartz\0";
    char f1P3Name[PASSENGER_NAME_MAX_LENGTH] = "Linus Thorvalds\0";
    addPassenger(f1, 0, f1P1Name, 55);
    addPassenger(f1, 0, f1P2Name, 23);
    addPassenger(f1, 0, f1P3Name, 56);

    char f2P1Name[PASSENGER_NAME_MAX_LENGTH] = "Alan Turing\0";
    char f2P2Name[PASSENGER_NAME_MAX_LENGTH] = "Bill Gates\0";
    char f2P3Name[PASSENGER_NAME_MAX_LENGTH] = "Linus Thorvalds\0";
    addPassenger(f2, 0, f2P1Name, 21);
    addPassenger(f2, 0, f2P2Name, 42);
    addPassenger(f2, 0, f2P3Name, 65);

    char f6P1Name[PASSENGER_NAME_MAX_LENGTH] = "Tim Berners-Lee\0";
    char f6P2Name[PASSENGER_NAME_MAX_LENGTH] = "James Gosling\0";
    char f6P3Name[PASSENGER_NAME_MAX_LENGTH] = "Brian Kernighan\0";
    addPassenger(f6, 0, f6P1Name, 21);
    addPassenger(f6, 0, f6P2Name, 42);
    addPassenger(f6, 0, f6P3Name, 88);

    printf("Program now uses predefined data.\r\n");
}

void promptFindFlightByIndex(FlightList *flightList) {
    printf("Find flight by index.\r\n");
    printf("Enter index: ");
    unsigned short index;
    int indexInputRes = scanf("%hu", &index);
    if(indexInputRes != 1) {
        printf("An error occured inputing index.");
        return;
    }

    Flight *flight = getFlightByIndex(flightList, index);

    if(flight == NULL) 
        return;

    printFlight(flight);
}

void promptFindFlightByDepartureTime(FlightList *flightList) {
    printf("Find flight by departure time.\r\n");
    printf("Enter departure: ");
    unsigned short departureTime;
    int departureTimeInputRes = scanf("%hu", &departureTime);
    if(departureTimeInputRes != 1) {
        printf("An error occured inputing departure time.");
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
    printf("Delete flight.\r\n");
    char flightId[FLIGHT_ID_MAX_LENGTH];
    printf("Enter flight id: ");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    if(deleteFlight(pFlightList, flightId)) 
        printf("Successfully deleted flight \"%s\"\r\n", flightId);
    else 
        printf("Failed deleted flight with id of: \"%s\"\r\n", flightId);
}