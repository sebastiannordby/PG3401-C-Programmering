#include "flight-booking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRINGIFY(x) #x

const unsigned short MN_ADD_FLIGHT = 1;
const unsigned short MN_ADD_PASSENGER = 2;
const unsigned short MN_PRINT_FLIGHT = 3;
const unsigned short MN_FIND_FLIGHT_BY_TIME = 4;
const unsigned short MN_DELETE_FLIGHT = 5;
const unsigned short MN_REMOVE_PASSENGER_RES = 6;
const unsigned short MN_CHANGE_PASSENGER_SEAT = 7;
const unsigned short MN_SEARCH_PASSENGER = 8;
const unsigned short MN_EXIT_PROGRAM = 9;

int readMenuChoice(void);
void printMenu(void);
void promptAddFlight(FlightList *flightList);
void promptAddPassenger(FlightList *flightList);
void promptPrintFlight(FlightList *flightList);
void promptFindFlightByTime(FlightList *flightList);
void promptDeleteFlight(FlightList *flightList);
void promptRemovePassengerRes(FlightList *flightList);
void promptChangePassengerSeat(FlightList *flightList);
void promptSearchPassenger(FlightList *flightList);
void freeFlightList(FlightList *flightList);
void readString(char *str, int maxLength);

int main(void) {
    FlightList *flightList = malloc(sizeof(FlightList));
    if(flightList == NULL) {
        printf("Could not create flight list.\r\n");
        return 1;
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
                promptPrintFlight();
                break;
            // case MN_FIND_FLIGHT_BY_TIME:
            //     promptFindFlightByTime();
            //     break;
            // case MN_DELETE_FLIGHT:
            //     promptDeleteFlight();
            //     break;
            // case MN_REMOVE_PASSENGER_RES:
            //     promptRemovePassengerRes();
            //     break;
            // case MN_CHANGE_PASSENGER_SEAT:
            //     promptChangePassengerSeat();
            //     break;
            // case MN_SEARCH_PASSENGER:
            //     promptSearchPassenger();
            //     break;
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
    printf("%d. Delete a flight\r\n", MN_DELETE_FLIGHT);
    printf("%d. Remove passenger reservation\r\n", MN_REMOVE_PASSENGER_RES);
    printf("%d. Change seat for passenger\r\n", MN_CHANGE_PASSENGER_SEAT);
    printf("%d. Search for passenger\r\n", MN_SEARCH_PASSENGER);
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
}
