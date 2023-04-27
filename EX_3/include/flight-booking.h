#ifndef __FLIGHT_BOOKING_H__
#define __FLIGHT_BOOKING_H__
#define PASSENGER_MAX_NAME 100
#include <stdbool.h>
/*
    struct representing a aassenger on a flight.
*/
typedef struct Passenger {
    int seatNumber;
    char name[PASSENGER_MAX_NAME];
    int age;
    struct Passenger *next;
} Passenger;

/*
    struct representing a flight.
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

Flight* addFlight(FlightList* flightList, char* flightID, char* destination, int numberOfSeats, int departureTime);
bool addPassenger(Flight *flight, int seatNumber, char name[PASSENGER_MAX_NAME], int age);

void printFlightList(FlightList *flightList);
void printPassengerList(Flight *flight);

#endif