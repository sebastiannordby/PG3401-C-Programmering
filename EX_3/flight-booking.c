#include "include/flight-booking.h"
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

unsigned int getAssociatedFlightsByPassengerName(const FlightList *pFlightList, 
    char passengerName[PASSENGER_NAME_MAX_LENGTH]) {

    if(pFlightList == NULL || passengerName == NULL || pFlightList->head == NULL)
        return 0;

    unsigned int numberOfAssociatedFlights = 0;
    Flight* currentFlight = pFlightList->head;

    while (currentFlight != NULL) {
        Passenger* currentPassenger = currentFlight->pPassengers;
        
        while (currentPassenger != NULL) {
             // If there is a person with the given name on the flight,
             // there is no need to check remaining passengers
            if (strcasecmp(currentPassenger->name, passengerName) == 0) {
                numberOfAssociatedFlights++;
                break;
            }

            currentPassenger = currentPassenger->pNext;
        }

        currentFlight = currentFlight->pNext;
    }

    return numberOfAssociatedFlights;
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
    if(flightList == NULL || flightList->head == NULL){
        printf("List of flight is empty.\r\n");
        return;
    }

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
    Adds a flight to the flight list.
*/
Flight* addFlight(
    FlightList* flightList, 
    char flightID[FLIGHT_ID_MAX_LENGTH], 
    char destination[FLIGHT_DESTINATION_MAX_LENGTH], 
    unsigned short numberOfSeats, 
    unsigned short departureTime) {

    // Create the flight struct    
    Flight* newFlight = malloc(sizeof(Flight));
    strcpy(newFlight->flightID, flightID);
    strcpy(newFlight->destination, destination);
    newFlight->numberOfSeats = numberOfSeats;
    newFlight->departureTime = departureTime;
    newFlight->pNext = NULL;
    newFlight->pPrev = NULL;
    newFlight->pPassengers = NULL;

    // If there is no elements, head is null
    // then we can assign the head to be the created flight
    if (flightList->head == NULL) {
        flightList->head = newFlight;
        flightList->tail = newFlight;
    } else {
        flightList->tail->pNext = newFlight;
        newFlight->pPrev = flightList->tail;
        flightList->tail = newFlight;
    }

    return newFlight;
}

bool deleteFlight(FlightList *pFlightList, char flightId[FLIGHT_ID_MAX_LENGTH]) {
    if(pFlightList == NULL || pFlightList->head == NULL)
        return false;

    Flight *pFlightToDelete = pFlightList->head;

    while (pFlightToDelete != NULL) {
        if(strcasecmp(pFlightToDelete->flightID, flightId) == 0) {
            // If the first flight is removed  
            // Set the next flight to be the head
            if(pFlightToDelete == pFlightList->head) {
                pFlightList->head = pFlightToDelete->pNext;
            } 
            
            // If the last flight is removed
            // Set the previous flight to be the tail
            if(pFlightToDelete == pFlightList->tail) {
                pFlightList->tail = pFlightToDelete->pPrev;
            } 

            // If there is a flight before the target in the list:
            // Sets it's previous to be the targeteds nexts.
            if(pFlightToDelete->pPrev != NULL) {
                pFlightToDelete->pPrev->pNext = pFlightToDelete->pNext;
            }

            // If there is a flight after the targeted in the list:
            // Set it's next to be the targeteds flights previous.
            if(pFlightToDelete->pNext != NULL) {
                pFlightToDelete->pNext->pPrev = pFlightToDelete->pPrev;
            }

            Passenger *pPassenger = pFlightToDelete->pPassengers;

            while (pPassenger != NULL) {
                Passenger *pPassengerTemp = pPassenger;
                pPassenger = pPassenger->pNext;
                free(pPassengerTemp);
            }

            free(pFlightToDelete);
            pFlightToDelete = NULL;

            return true;
        }

        pFlightToDelete = pFlightToDelete->pNext;
    }
    
    return false;
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
bool addPassenger(Flight *pFlight, unsigned short requestedSeatNumber, 
    char name[PASSENGER_NAME_MAX_LENGTH], unsigned short age) {
    
    // Check if arguments passed is valid
    if(pFlight == NULL || name == NULL)
        return false;

    Passenger *newPassenger = malloc(sizeof(Passenger));
    newPassenger->age = age;
    newPassenger->seatNumber = requestedSeatNumber != 0 ? requestedSeatNumber : 1;
    strcpy(newPassenger->name, name);
    newPassenger->name[PASSENGER_NAME_MAX_LENGTH - 1] = '\0';

    if(pFlight->pPassengers == NULL) {
        pFlight->pPassengers = newPassenger;
    } else if(requestedSeatNumber > 0) {
        Passenger *closestPassenger = getPassengerClosestToSeatNumber(
            pFlight, requestedSeatNumber);

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
        Passenger *currentPassenger = pFlight->pPassengers;

        while(currentPassenger != NULL && 
            currentPassenger->pNext != NULL) {
            currentPassenger = currentPassenger->pNext;
        }

        if(currentPassenger->pNext == NULL && 
            currentPassenger->seatNumber + 1 < pFlight->numberOfSeats) {
            newPassenger->seatNumber = currentPassenger->seatNumber + 1;
        }

        newPassenger->pNext = currentPassenger->pNext;
        currentPassenger->pNext = newPassenger;
    }

    return true;
}

bool removePassenger(
    FlightList *pFlightList,
    char flightId[FLIGHT_ID_MAX_LENGTH], unsigned short seatNumber) {
    if(flightId == NULL || seatNumber == 0)
        return false;

    Flight *pFlight = getFlightById(pFlightList, flightId);
    if(pFlight == NULL)
        return false;

    Passenger *pCurrentPassenger = pFlight->pPassengers;
    Passenger *pPreviousPassenger = NULL;

    while(pCurrentPassenger != NULL) {
        if(pCurrentPassenger->seatNumber == seatNumber) {
            if(pPreviousPassenger != NULL) {
                pPreviousPassenger->pNext = pCurrentPassenger->pNext;
            }

            free(pCurrentPassenger);
            pCurrentPassenger = NULL;

            return true;
        }

        pPreviousPassenger = pCurrentPassenger;
        pCurrentPassenger = pCurrentPassenger->pNext;
    }

    return false;
}


bool changePassengerSeat(FlightList *pFlightList, char flightId[FLIGHT_ID_MAX_LENGTH],
    unsigned short currentSeatNumber, unsigned short requestedSeatNumber) {

    if(pFlightList == NULL || flightId == NULL)
        return false;

    Flight *pFlight = getFlightById(pFlightList, flightId);
    if(pFlight == NULL)
        return false;

    Passenger *pPassengerList = pFlight->pPassengers;

    // Find the passenger with the current seat number
    Passenger *currentPassenger = pPassengerList;
    while (currentPassenger != NULL && currentPassenger->seatNumber != currentSeatNumber) {
        currentPassenger = currentPassenger->pNext;
    }
    
    if (currentPassenger == NULL) 
        return false;
    
    // Check if the requested seat number is already taken
    Passenger *tempPassenger = pPassengerList;
    while (tempPassenger != NULL && tempPassenger->seatNumber != requestedSeatNumber) {
        tempPassenger = tempPassenger->pNext;
    }
    if (tempPassenger != NULL) {
        return false;
    }
    
    // Update the seat number of the targeted passenger
    currentPassenger->seatNumber = requestedSeatNumber;
    
    // Remove the targeted passenger from the list
    if (currentPassenger == pPassengerList) {
        pPassengerList = currentPassenger->pNext;
    } else {
        Passenger *prevPassenger = pPassengerList;
        while (prevPassenger->pNext != currentPassenger) {
            prevPassenger = prevPassenger->pNext;
        }
        prevPassenger->pNext = currentPassenger->pNext;
    }
    
    // Re-insert the targeted passenger into the list at the correct position
    Passenger *prevPassenger = NULL;
    tempPassenger = pPassengerList;
    while (tempPassenger != NULL && tempPassenger->seatNumber < requestedSeatNumber) {
        prevPassenger = tempPassenger;
        tempPassenger = tempPassenger->pNext;
    }
    if (prevPassenger == NULL) {
        currentPassenger->pNext = pPassengerList;
        pPassengerList = currentPassenger;
    } else {
        currentPassenger->pNext = prevPassenger->pNext;
        prevPassenger->pNext = currentPassenger;
    }

    pFlight->pPassengers = pPassengerList;
    
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
    Ignores case sensitivity.
*/
Flight* getFlightById(FlightList *pFlightList, char flightId[FLIGHT_ID_MAX_LENGTH]) {
    if(pFlightList == NULL || flightId == NULL)
        return NULL;

    Flight *currentFlight = pFlightList->head;

    // Loops until the flight is found, or 
    // an occurence of NULL which suggest that there is no flight
    // with the provided id.
    while(currentFlight != NULL && strcasecmp(currentFlight->flightID, flightId) != 0) {
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

Flight* getFlightByDepartureTime(FlightList *pFlightList, unsigned short time) {
    if(pFlightList == NULL)
        return NULL;

    Flight *currentFlight = pFlightList->head;

    while(currentFlight != NULL && currentFlight->departureTime != time) {
        currentFlight = currentFlight->pNext;
    }

    return currentFlight;
}
