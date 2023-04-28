#ifndef __FLIGHT_BOOKING_H__
#define __FLIGHT_BOOKING_H__
#define PASSENGER_NAME_MAX_LENGTH 100
#define FLIGHT_ID_MAX_LENGTH 10
#define FLIGHT_DESTINATION_MAX_LENGTH 50
#include <stdbool.h>
/*
    struct representing a aassenger on a flight.
*/
typedef struct Passenger {
    int seatNumber;
    char name[PASSENGER_NAME_MAX_LENGTH];
    int age;
    struct Passenger *next;
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
    int numberOfSeats;
    int departureTime;
    struct Flight *next;
    struct Flight *prev;
    struct Passenger *passengers;
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
    Adds a flight to the list.
*/

Flight* addFlight(FlightList* flightList, char flightId[FLIGHT_ID_MAX_LENGTH], 
    char destination[FLIGHT_DESTINATION_MAX_LENGTH], int numberOfSeats, int departureTime);

bool addPassenger(Flight *flight, 
    int seatNumber, char name[PASSENGER_NAME_MAX_LENGTH], int age);

void printFlightList(const FlightList *flightList);
void printFlight(const Flight *flight);
void printPassengerList(const Flight *flight);

#endif