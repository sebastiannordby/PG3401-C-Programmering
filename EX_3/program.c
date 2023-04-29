#include "flight-booking.h"
#include "dummy-flight-data.h"
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
void freeFlightList(FlightList *flightList);
void readString(char *str, int maxLength);
void printMenu(void);
void printMenuHeader(char *header);
void printFailedInput(char *inputName);
void promptEnterInput(char *inputName);
void promptAddFlight(FlightList *flightList);
void promptAddPassenger(FlightList *flightList);
void promptPrintFlight(FlightList *flightList);
void promptFindFlightByDepartureTime(FlightList *flightList);
void promptFindFlightByIndex(FlightList *flightList);
void promptDeleteFlight(FlightList *flightList);
void promptRemovePassengerRes(FlightList *flightList);
void promptChangePassengerSeat(FlightList *flightList);
void promptSearchPassenger(FlightList *flightList);

int main(int argc, char *argv[]) {
    FlightList *flightList = malloc(sizeof(FlightList));
    if(flightList == NULL) {
        printf("Could not create flight list.\r\n");
        return 1;
    }

    // If there are any arguments run with preloaded
    // flight data. Easier for debugging purposes.
    if(argc > 0) 
        usePredefinedDummyData(flightList);


    // Handle the menu prompting and delegation of which
    // operation the user has choosen.
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
            case MN_SEARCH_PASSENGER:
                promptSearchPassenger(flightList);
                break;
            default:
                printf("Unvalid menu choice.\r\n");
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

void promptEnterInput(char *inputName) {
    printf("Enter %s: ", inputName);
}

void promptAddFlight(FlightList *flightList) {
    printMenuHeader("ADD FLIGHT");

    char flightId[FLIGHT_ID_MAX_LENGTH];
    promptEnterInput("id");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    char destination[FLIGHT_DESTINATION_MAX_LENGTH];
    promptEnterInput("destination");
    readString(destination, FLIGHT_DESTINATION_MAX_LENGTH);

    unsigned short numberOfSeats;
    promptEnterInput("number of seats");
    if(scanf("%hu", &numberOfSeats) != 1) {
        printFailedInput("number of seats");
        return;
    }

    unsigned short departureTime;
    promptEnterInput("departure time");
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
    promptEnterInput("flight id");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    Flight* flight = getFlightById(flightList, flightId);
    if(flight == NULL) {
        printf("No flight with id of: %s\r\n", flightId);
        return;
    }

    char name[PASSENGER_NAME_MAX_LENGTH];
    promptEnterInput("name");
    readString(name, PASSENGER_NAME_MAX_LENGTH);

    unsigned short age;
    promptEnterInput("age");
    if(scanf("%hu", &age) != 1) {
        printFailedInput("age");
        return;
    }

    unsigned short seatNumber;
    promptEnterInput("seat number(-1 for automatic allocation)");
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
    promptEnterInput("flight id");
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
    promptEnterInput("index");
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
    promptEnterInput("departure");
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
    promptEnterInput("flight id");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    if(deleteFlight(pFlightList, flightId)) 
        printf("Successfully deleted flight \"%s\"\r\n", flightId);
    else 
        printf("Failed to delete flight with id of: \"%s\"\r\n", flightId);
}

void promptRemovePassengerRes(FlightList *flightList) {
    printMenuHeader("REMOVE PASSENGER RESERVATION");

    char flightId[FLIGHT_ID_MAX_LENGTH];
    promptEnterInput("flight id");
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
    promptEnterInput("flight id");
    readString(flightId, FLIGHT_ID_MAX_LENGTH);

    printf("Enter current seat number: ");
    unsigned short currentSeatNumber;
    if(scanf("%hu", &currentSeatNumber) != 1) {
        printFailedInput("current seat number");
        return;
    }

    promptEnterInput("requested seat number");
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

void promptSearchPassenger(FlightList *pFlightList) {
    printMenuHeader("SEARCH PASSENGER FLIGHTS");
    char passengerName[PASSENGER_NAME_MAX_LENGTH];
    promptEnterInput("name");
    readString(passengerName, PASSENGER_NAME_MAX_LENGTH);

    unsigned int numberOfFlights = 
        getAssociatedFlightsByPassengerName(pFlightList, passengerName);

    printf("Number of flights accosiated with name: %s is %hu\r\n", passengerName, numberOfFlights);
}

