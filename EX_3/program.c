#include "flight-booking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void promptAddPassenger(void);
void promptPrintFlight(void);
void promptFindFlightByTime(void);
void promptDeleteFlight(void);
void promptRemovePassengerRes(void);
void promptChangePassengerSeat(void);
void promptSearchPassenger(void);

void promptAddFlight(FlightList *flightList) {
    printf("---------- Add flight ----------\r\n");

    char flightId[FLIGHT_ID_MAX_LENGTH];
    printf("Enter id: ");
    int flightIdRes = scanf("%s", flightId);

    char destination[FLIGHT_DESTINATION_MAX_LENGTH];
    printf("Enter destination: ");
    int destinationRes = scanf("%s", destination);

    int numberOfSeats;
    printf("Enter number of seats: ");
    int numberOfSeatsRes = scanf("%d", &numberOfSeats);

    int departureTime;
    printf("Enter departure time: ");
    int departureTimeRes = scanf("%d", &departureTime);

    Flight *flight = addFlight(flightList, flightId, destination, numberOfSeats, departureTime);

    if(flight != NULL) {
        printf("Successfully created flight \"%s\"\r\n", flightId);
        printf("Here is the updated flight list:\r\n");
        printFlightList(flightList);
        printf("\r\n");
    }
}


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
            // case MN_ADD_PASSENGER:
            //     promptAddPassenger();
            //     break;
            // case MN_PRINT_FLIGHT:
            //     promptPrintFlight();
            //     break;
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
    printf("booking$ ");
    int choice;
    int scanFRes = scanf("%d", &choice);

    return scanFRes != EOF ? choice : EOF;
}

// int main(void) {
//     FlightList *flightList = malloc(sizeof(FlightList));
//     if(flightList == NULL) {
//         printf("Could not create flight list.\r\n");
//         return 1;
//     }

//     Flight* f1 = addFlight(flightList, "S8993", "Oslo Lufthavn", 22, 1020);
//     Flight* f2 = addFlight(flightList, "Z2938", "Gdansk", 22, 1020);
//     Flight* f3 = addFlight(flightList, "G2938", "Alanya", 22, 1020);
//     Flight* f4 = addFlight(flightList, "D2938", "Copenhagen", 22, 1020);

//     printFlightList(flightList);

//     printf("\r\nAdding passengers\r\n");

//     addPassenger(f1, 1, "Ola Normann\0", 25);
//     addPassenger(f1, 2, "Heidi Olsen\0", 23);
//     addPassenger(f1, -1, "Erik Jenssen\0", 23);

//     printPassengerList(f1);

//     return 0;
// }

