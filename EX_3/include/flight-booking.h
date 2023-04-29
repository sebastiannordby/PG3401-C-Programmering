#ifndef __FLIGHT_BOOKING_H__
#define __FLIGHT_BOOKING_H__

#include <stdbool.h>
#define PASSENGER_NAME_MAX_LENGTH 50
#define FLIGHT_ID_MAX_LENGTH 10
#define FLIGHT_DESTINATION_MAX_LENGTH 50

// Struct representation of a passenger on a flight.
typedef struct Passenger {
    unsigned short seatNumber;
    char name[PASSENGER_NAME_MAX_LENGTH];
    unsigned short age;
    struct Passenger *pNext;
} Passenger;

// Struct representation of a flight.
typedef struct Flight {
    char flightID[10];
    char destination[50];
    unsigned short numberOfSeats;
    unsigned short departureTime;
    struct Flight *pNext;
    struct Flight *pPrev;
    struct Passenger *pPassengers;
} Flight;

// Struct representing double linked list over flights.
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

// Removes a flight from flightlist.
bool deleteFlight(
    FlightList *pFlightList, 
    char flightId[FLIGHT_ID_MAX_LENGTH]);

// Finds a flight with given id.
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

// Adds a passenger to given flight.
bool addPassenger(
    Flight *pFlightList, 
    unsigned short requestedSeatNumber, 
    char name[PASSENGER_NAME_MAX_LENGTH], 
    unsigned short age);

// Removes a passenger from given flight.
bool removePassenger(
    FlightList *pFlightList,
    char flightId[FLIGHT_ID_MAX_LENGTH],
    unsigned short seatNumber);

// Changes seat for given passenger, on given flight.
bool changePassengerSeat(
    FlightList *pFlightList,
    char flightId[FLIGHT_ID_MAX_LENGTH],
    unsigned short currentSeatNumber,
    unsigned short requestedSeatNumber);

//Prints out a formated flight list.
void printFlightList(
    const FlightList *pFlightList);

// Prints out a formated passenger list for the given flight.
void printPassengerList(
    const Flight *pFlight);

// Print out a single flight
void printFlight(
    const Flight *pFlight);

// Finds the number of accosiated flights by the passengers name.
unsigned int getAssociatedFlightsByPassengerName(
    const FlightList *pFlightList,
    char passengerName[PASSENGER_NAME_MAX_LENGTH]);
#endif