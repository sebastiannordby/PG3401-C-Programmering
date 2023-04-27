#include "flight-booking.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

/*
    Prints out a flight in the following format:
    Id: @ Destination: @ Depature: @
*/
void printFlight(Flight *flight) {
    printf("%-10s| %-50s| %d\r\n", 
        flight->flightID, flight->destination, flight->departureTime);
}

/*
    Prints out the list of flights in the following format:
    Id: @ Destination: @ Depature: @
*/

void printDottedLine(int length) {
    for(int i = 0; i <= 10 + 50 + 10; i++) 
        putchar('-');

    putchar('\n');
}

void printFlightList(FlightList* flightList) {
    printDottedLine(10 + 50 + 10);
    printf("%-10s| %-50s| %-10s\r\n", "Id", "Destination", "Depature");
    printDottedLine(10 + 50 + 10);

    Flight *currentFlight = flightList->head;
    while (currentFlight != NULL) {
        printFlight(currentFlight);
        currentFlight = currentFlight->next;
    }
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
Flight* addFlight(FlightList* flightList, char* flightID, 
    char* destination, int numberOfSeats, int departureTime) {
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

bool addPassenger(Flight *flight, int requestedSeatNumber, 
    char name[PASSENGER_MAX_NAME], int age) {
    
    // Check if arguments passed is valid
    if(flight == NULL || name == NULL || age < 0)
        return false;

    Passenger *newPassenger = malloc(sizeof(Passenger));
    newPassenger->age = age;
    newPassenger->seatNumber = requestedSeatNumber;
    strncpy(newPassenger->name, name, sizeof(newPassenger->name));

    if(flight->passengers == NULL) {
        flight->passengers = newPassenger;
    } else {
        Passenger *currentPassenger = flight->passengers;

        while(currentPassenger != NULL && 
            currentPassenger->seatNumber < requestedSeatNumber && 
            currentPassenger->next != NULL) {
            currentPassenger = currentPassenger->next;
        }

        // Tried to use an existing seatnumber
        if(isSeatTaken(currentPassenger, requestedSeatNumber)) {
            printf("Seat: %d is already taken.\r\n", requestedSeatNumber);
            free(newPassenger);
            newPassenger = NULL;
            return false;
        }

        newPassenger->next = currentPassenger->next;
        currentPassenger->next = newPassenger;
    }

    return true;
}


void printPassengerList(Flight *flight) {
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