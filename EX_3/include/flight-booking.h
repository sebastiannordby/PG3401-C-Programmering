#ifndef __FLIGHT_BOOKING_H__
#define __FLIGHT_BOOKING_H__

#include <stdbool.h>
#define PASSENGER_NAME_MAX_LENGTH 50
#define FLIGHT_ID_MAX_LENGTH 10
#define FLIGHT_DESTINATION_MAX_LENGTH 50

/*
    struct representing a passenger on a flight.
*/
typedef struct Passenger {
    unsigned short seatNumber;
    char name[PASSENGER_NAME_MAX_LENGTH];
    unsigned short age;
    struct Passenger *pNext;
} Passenger;

/*
    struct representing a flight.
    Examples of a flight: 
    flightId: BA-42
    destination: Oslo Airport
    numberOfSeats: 98
    depatureTime: 1020
*/
typedef struct Flight {
    char flightID[10];
    char destination[50];
    unsigned short numberOfSeats;
    unsigned short departureTime;
    struct Flight *pNext;
    struct Flight *pPrev;
    struct Passenger *pPassengers;
} Flight;

/*
    Double linked list to keep track
    of the flights. 
*/
typedef struct FlightList {
    struct Flight *head;
    struct Flight *tail;
} FlightList;

/*
    Adds a flight to the list flightList.
*/
Flight* addFlight(
    FlightList *pFlightList, 
    char flightId[FLIGHT_ID_MAX_LENGTH], 
    char destination[FLIGHT_DESTINATION_MAX_LENGTH], 
    unsigned short numberOfSeats, 
    unsigned short departureTime);

/*
    Returns the flight with the given id.
*/
Flight* getFlightById(
    FlightList *pFlightList, 
    char flightId[FLIGHT_ID_MAX_LENGTH]);

Flight* getFlightByIndex(
    FlightList *pFlightList,
    unsigned short index);

/*
    Adds a passenger to the given flight.
    If requestesSeatNumber is -1, automatically "allocate" seat.
*/
bool addPassenger(
    Flight *pFlightList, 
    unsigned short requestedSeatNumber, 
    char name[PASSENGER_NAME_MAX_LENGTH], 
    unsigned short age);

/*
    Prints out a formated flight list.
*/
void printFlightList(
    const FlightList *pflightList);

/*
    Prints out a formated passenger list 
    for the given flight.
*/
void printPassengerList(
    const Flight *flight);

void printFlight(
    const Flight *flight);

/*
    Returns an array of available seats, and the size of the array.
*/
int* getAvailableSeats(
    const Flight *pFlight, 
    unsigned short *size);

#endif