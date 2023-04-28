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

// Adds a flight to the list flightList.
Flight* addFlight(
    FlightList *pFlightList, 
    char flightId[FLIGHT_ID_MAX_LENGTH], 
    char destination[FLIGHT_DESTINATION_MAX_LENGTH], 
    unsigned short numberOfSeats, 
    unsigned short departureTime);

// Removes a flight from the flightlist.
bool deleteFlight(
    FlightList *pFlightList, 
    char flightId[FLIGHT_ID_MAX_LENGTH]);

// Finds a flight with the given id.
Flight* getFlightById(
    FlightList *pFlightList, 
    char flightId[FLIGHT_ID_MAX_LENGTH]);

// Finds a flight by index.
Flight* getFlightByIndex(
    FlightList *pFlightList,
    unsigned short index);

// Finds a flight by departure time.
Flight* getFlightByDepartureTime(
    FlightList *pFlightList,
    unsigned short time);

// Adds a passenger to the given flight.
bool addPassenger(
    Flight *pFlightList, 
    unsigned short requestedSeatNumber, 
    char name[PASSENGER_NAME_MAX_LENGTH], 
    unsigned short age);

// Removes a passenger from the given flight.
bool removePassenger(
    char flightId[FLIGHT_ID_MAX_LENGTH],
    unsigned short seatNumber);

// Returns an array of available seats, and the size of the array.
int* getAvailableSeats(
    const Flight *pFlight, 
    unsigned short *size);

//Prints out a formated flight list.
void printFlightList(
    const FlightList *pFlightList);

// Prints out a formated passenger list for the given flight.
void printPassengerList(
    const Flight *pFlight);

// Print out a single flight
void printFlight(
    const Flight *pFlight);

#endif