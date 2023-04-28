#include "flight-booking.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// Declare functions to skip precendence in code
void printDottedLine(int length);

/*
    Prints out a flight in the following format:
    {id} | {destination} | {departure}

    Then the passenger list in the following format:
    {seat} | {name} | {age}
*/
void printFlight(const Flight *flight) {
    printDottedLine(100);
    printf("%-10s| %-50s| %-10s | %-10s\r\n", 
        "Id", "Destination", "Depature", "Seats");
    printf("%-10s| %-50s| %-10d | %-10d\r\n", 
        flight->flightID, flight->destination, 
        flight->departureTime, flight->numberOfSeats);
    printDottedLine(100);
    
    if(flight->pPassengers != NULL) {
        Passenger *currentPassenger = flight->pPassengers;
        printf("%-5s| %-50s| %-3s\r\n", 
            "Seat", "Name", "Age");

        while(currentPassenger != NULL) {
            printf("%-5hu| %-50s| %-3hu\r\n",
                currentPassenger->seatNumber,
                currentPassenger->name,
                currentPassenger->age);

            currentPassenger = currentPassenger->pNext;
        }
    }

    printDottedLine(100);
}

/*
    Prints out a dotted line in the provided length
*/
void printDottedLine(int length) {
    if(length <= 0)
        return;

    for(int i = 0; i <= length; i++) 
        putchar('-');

    putchar('\n');
}

/*
    Prints out the flight list formated.
    Ex:
    -----------------------------------------------------------------------------------
    Id        | Destination                                       | Depature   | Seats     
    -----------------------------------------------------------------------------------
    SK1234    | Oslo Airport                                      | 1930      | 99     
    SK4321    | Gdansk Airport                                    | 1930      | 79     
    -----------------------------------------------------------------------------------
*/
void printFlightList(const FlightList* flightList) {
    int numCharsDottedLine = 82;
    printDottedLine(numCharsDottedLine);
    printf("%-10s| %-50s| %-10s | %-10s\r\n", "Id", "Destination", "Depature", "Seats");
    printDottedLine(numCharsDottedLine);

    Flight *currentFlight = flightList->head;
    while (currentFlight != NULL) {
        printf("%-10s| %-50s| %-10d | %-10d\r\n", 
            currentFlight->flightID, currentFlight->destination, 
            currentFlight->departureTime, currentFlight->numberOfSeats);
        currentFlight = currentFlight->pNext;
    }

    printDottedLine(numCharsDottedLine);
}


/*
    Creates a flight struct and returns it's pointer.
*/
Flight* createFlight(
    char* flightID,
    char* destination, 
    unsigned short numberOfSeats, 
    unsigned short departureTime) {

    Flight* newFlight = malloc(sizeof(Flight));
    strncpy(newFlight->flightID, flightID, sizeof(newFlight->flightID));
    strncpy(newFlight->destination, destination, sizeof(newFlight->destination));
    newFlight->numberOfSeats = numberOfSeats;
    newFlight->departureTime = departureTime;
    newFlight->pNext = NULL;
    newFlight->pPrev = NULL;
    newFlight->pPassengers = NULL;

    return newFlight;
}

/*
    Adds a flight to the flight list.

*/
Flight* addFlight(
    FlightList* flightList, 
    char flightID[FLIGHT_ID_MAX_LENGTH], 
    char destination[FLIGHT_DESTINATION_MAX_LENGTH], 
    unsigned short numberOfSeats, 
    unsigned short departureTime) {

    // Create the flight struct    
    Flight *newFlight = createFlight(flightID, destination, numberOfSeats, departureTime);

    // If there is no elements, head is null
    // then we can assign the head to be the created flight
    if (flightList->head == NULL) {
        flightList->head = newFlight;
        flightList->tail = newFlight;
    } else {
        flightList->tail->pNext = newFlight;
        flightList->tail = newFlight;
    }

    return newFlight;
}

bool isSeatTaken(Passenger *p, unsigned short seatNumber) {
    if(p == NULL)
        return false;

    if(p->seatNumber == seatNumber)
        return true;

    if(p->pNext != NULL && p->pNext->seatNumber == seatNumber)
        return true;

    return false;
}

/*
    Returns the passenger closest to the seat number provided.
*/
Passenger* getPassengerClosestToSeatNumber(Flight *flight, unsigned short seatNumber) {
    Passenger *currentPassenger = flight->pPassengers;

    while(currentPassenger != NULL && 
        currentPassenger->seatNumber < seatNumber && 
        currentPassenger->pNext != NULL) {
        currentPassenger = currentPassenger->pNext;
    }

    return currentPassenger;
}

/*
    Adds a passenger to the given flight.
    If the requstedSeatNumber is 0, take the first available.
*/
bool addPassenger(Flight *flight, unsigned short requestedSeatNumber, 
    char name[PASSENGER_NAME_MAX_LENGTH], unsigned short age) {
    
    // Check if arguments passed is valid
    if(flight == NULL || name == NULL)
        return false;

    Passenger *newPassenger = malloc(sizeof(Passenger));
    newPassenger->age = age;
    newPassenger->seatNumber = requestedSeatNumber != 0 ? requestedSeatNumber : 1;
    strncpy(newPassenger->name, name, sizeof(newPassenger->name));

    if(flight->pPassengers == NULL) {
        flight->pPassengers = newPassenger;
    } else if(requestedSeatNumber > 0) {
        Passenger *closestPassenger = getPassengerClosestToSeatNumber(
            flight, requestedSeatNumber);

        // Tried to use an existing seatnumber
        if(isSeatTaken(closestPassenger, requestedSeatNumber)) {
            printf("Seat: %d is already taken.\r\n", requestedSeatNumber);
            free(newPassenger);
            newPassenger = NULL;
            return false;
        }

        newPassenger->pNext = closestPassenger->pNext;
        closestPassenger->pNext = newPassenger;
    } else {
        Passenger *currentPassenger = flight->pPassengers;

        while(currentPassenger != NULL && 
            currentPassenger->pNext != NULL) {
            currentPassenger = currentPassenger->pNext;
        }

        if(currentPassenger->pNext == NULL && 
            currentPassenger->seatNumber + 1 < flight->numberOfSeats) {
            newPassenger->seatNumber = currentPassenger->seatNumber + 1;
        }

        newPassenger->pNext = currentPassenger->pNext;
        currentPassenger->pNext = newPassenger;
    }

    return true;
}


void printPassengerList(const Flight *pFlight) {
    if(pFlight == NULL || pFlight->pPassengers == NULL)
        return;

    Passenger *currentPassenger = pFlight->pPassengers;

    while(currentPassenger != NULL) {
        printf("Seat: %d\tName: %s\r\n", 
            currentPassenger->seatNumber,
            currentPassenger->name);

        currentPassenger = currentPassenger->pNext;
    }
}

/*
    Returns a flight by the given flightId.
*/
Flight* getFlightById(FlightList *pFlightList, char flightId[FLIGHT_ID_MAX_LENGTH]) {
    if(pFlightList == NULL || flightId == NULL)
        return NULL;

    Flight *currentFlight = pFlightList->head;

    // Loops until the flight is found, or 
    // an occurence of NULL which suggest that there is no flight
    // with the provided id.
    while(currentFlight != NULL && strcmp(currentFlight->flightID, flightId) != 0) {
        currentFlight = currentFlight->pNext;
    }

    return currentFlight;
}

/*
    Returns a flight at the given index. 
    Indexing starts as 1.
*/
Flight* getFlightByIndex(FlightList *pFlightList, unsigned short index) {
    if(pFlightList == NULL)
        return NULL;

    
    int currIndex = 1;
    Flight *currentFlight = pFlightList->head;

    while(currentFlight != NULL && currIndex <= index - 1) {
        currentFlight = currentFlight->pNext;
        currIndex++;
    }

    return currentFlight;
}



int* getAvailableSeats(const Flight *flight, unsigned short *size) {
    int *result = malloc(sizeof(int) * flight->numberOfSeats);

    if(flight->pPassengers == NULL) {

    }

    return NULL;
}

