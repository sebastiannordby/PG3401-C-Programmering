#include "flight-booking.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*
    Prints out a flight in the following format:
    Id: @ Destination: @ Depature: @
*/
void printFlight(const Flight *flight) {
    printf("%-10s| %-50s| %-10d | %-10d\r\n", 
        flight->flightID, flight->destination, 
        flight->departureTime, flight->numberOfSeats);
}

/*
    Prints out the list of flights in the following format:
    Id: @ Destination: @ Depature: @
*/

void printDottedLine(int length) {
    for(int i = 0; i <= length; i++) 
        putchar('-');

    putchar('\n');
}

void printFlightList(const FlightList* flightList) {
    int numCharsDottedLine = 82;
    printDottedLine(numCharsDottedLine);
    printf("%-10s| %-50s| %-10s | %-10s\r\n", "Id", "Destination", "Depature", "Seats");
    printDottedLine(numCharsDottedLine);

    Flight *currentFlight = flightList->head;
    while (currentFlight != NULL) {
        printFlight(currentFlight);
        currentFlight = currentFlight->next;
    }

    printDottedLine(numCharsDottedLine);
}

/*
    Creates a flight struct and returns it's pointer.
*/
Flight* createFlight(char* flightID, char* destination, int numberOfSeats, int departureTime) {
    Flight* newFlight = malloc(sizeof(Flight));
    strncpy(newFlight->flightID, flightID, sizeof(newFlight->flightID));
    strncpy(newFlight->destination, destination, sizeof(newFlight->destination));
    newFlight->numberOfSeats = numberOfSeats;
    newFlight->departureTime = departureTime;
    newFlight->next = NULL;
    newFlight->prev = NULL;
    newFlight->passengers = NULL;

    return newFlight;
}

/*
    Adds a flight to the flight list.

*/
Flight* addFlight(FlightList* flightList, char flightID[FLIGHT_ID_MAX_LENGTH], 
    char destination[FLIGHT_DESTINATION_MAX_LENGTH], int numberOfSeats, int departureTime) {
    // Create the flight struct    
    Flight *newFlight = createFlight(flightID, destination, numberOfSeats, departureTime);

    // If there is no elements, head is null
    // then we can assign the head to be the created flight
    if (flightList->head == NULL) {
        flightList->head = newFlight;
        flightList->tail = newFlight;
    } else {
        flightList->tail->next = newFlight;
        flightList->tail = newFlight;
    }

    return newFlight;
}

bool isSeatTaken(Passenger *p, int seatNumber) {
    if(p == NULL)
        return false;

    if(p->seatNumber == seatNumber)
        return true;

    if(p->next != NULL && p->next->seatNumber == seatNumber)
        return true;

    return false;
}

/*
    Returns the passenger closest to the seat number provided.
*/
Passenger* getPassengerClosestToSeatNumber(Flight *flight, int seatNumber) {
    Passenger *currentPassenger = flight->passengers;

    while(currentPassenger != NULL && 
        currentPassenger->seatNumber < seatNumber && 
        currentPassenger->next != NULL) {
        currentPassenger = currentPassenger->next;
    }

    return currentPassenger;
}

/*
    Adds a passenger to the given flight.
    If the requstedSeatNumber is -1, take the first available.
*/
bool addPassenger(Flight *flight, int requestedSeatNumber, 
    char name[PASSENGER_NAME_MAX_LENGTH], int age) {
    
    // Check if arguments passed is valid
    if(flight == NULL || name == NULL || age < 0)
        return false;

    Passenger *newPassenger = malloc(sizeof(Passenger));
    newPassenger->age = age;
    newPassenger->seatNumber = requestedSeatNumber;
    strncpy(newPassenger->name, name, sizeof(newPassenger->name));

    if(flight->passengers == NULL) {
        flight->passengers = newPassenger;
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

        newPassenger->next = closestPassenger->next;
        closestPassenger->next = newPassenger;
    } else {
        Passenger *currentPassenger = flight->passengers;

        while(currentPassenger != NULL && 
            currentPassenger->next != NULL) {
            currentPassenger = currentPassenger->next;
        }

        if(currentPassenger->next == NULL && 
            currentPassenger->seatNumber + 1 < flight->numberOfSeats) {
            newPassenger->seatNumber = currentPassenger->seatNumber + 1;
        }

        newPassenger->next = currentPassenger->next;
        currentPassenger->next = newPassenger;
    }

    return true;
}


void printPassengerList(const Flight *flight) {
    if(flight == NULL || flight->passengers == NULL)
        return;

    Passenger *currentPassenger = flight->passengers;

    while(currentPassenger != NULL) {
        printf("Seat: %d\tName: %s\r\n", 
            currentPassenger->seatNumber,
            currentPassenger->name);

        currentPassenger = currentPassenger->next;
    }
}