#ifndef __FLIGHT_BOOKING_H__
#define __FLIGHT_BOOKING_H__

#include <stdbool.h>
#define PASSENGER_NAME_MAX_LENGTH 50
#define FLIGHT_ID_MAX_LENGTH 10
#define FLIGHT_DESTINATION_MAX_LENGTH 50

// Struct representation of a passenger on a flight.
typedef struct Passenger {
    unsigned short seat_number;
    char name[PASSENGER_NAME_MAX_LENGTH];
    unsigned short age;
    struct Passenger *next;
} Passenger;

// Struct representation of a flight.
typedef struct Flight {
    char flightID[10];
    char destination[50];
    unsigned short number_of_seats;
    unsigned short departure_time;
    struct Flight *next;
    struct Flight *prev;
    struct Passenger *passengers;
} Flight;

// Struct representing double linked list over flights.
typedef struct FlightList {
    struct Flight *head;
    struct Flight *tail;
} FlightList;

// Adds a flight to the list flightList.
Flight* add_flight(
    FlightList *flight_list, 
    char flight_id[FLIGHT_ID_MAX_LENGTH], 
    char destination[FLIGHT_DESTINATION_MAX_LENGTH], 
    unsigned short number_of_seats, 
    unsigned short departure_time);

// Removes a flight from flightlist.
bool delete_flight(
    FlightList *flight_list, 
    char flight_id[FLIGHT_ID_MAX_LENGTH]);

// Finds a flight with given id.
Flight* get_flight_by_id(
    FlightList *flight_list, 
    char flight_id[FLIGHT_ID_MAX_LENGTH]);

// Finds a flight by index.
Flight* get_flight_by_index(
    FlightList *flight_list,
    unsigned short index);

// Finds a flight by departure time.
Flight* get_flight_by_departure_time(
    FlightList *flight_list,
    unsigned short time);

// Adds a passenger to given flight.
bool add_passenger(
    Flight *flight_list, 
    unsigned short requested_seat_number, 
    char name[PASSENGER_NAME_MAX_LENGTH], 
    unsigned short age);

// Removes a passenger from given flight.
bool remove_passenger(
    FlightList *flight_list,
    char flight_id[FLIGHT_ID_MAX_LENGTH],
    unsigned short seat_number);

// Changes seat for given passenger, on given flight.
bool change_passenger_seat(
    FlightList *flight_list,
    char flight_id[FLIGHT_ID_MAX_LENGTH],
    unsigned short current_seat_number,
    unsigned short requested_seat_number);

//Prints out a formated flight list.
void print_flight_list(
    const FlightList *flight_list);

// Prints out a formated passenger list for the given flight.
void print_passenger_list(
    const Flight *flight);

// Print out a single flight
void print_flight(
    const Flight *flight);

// Finds the number of accosiated flights by the passengers name.
unsigned int get_associated_flights_by_passenger_name(
    const FlightList *flight_list,
    char passenger_name[PASSENGER_NAME_MAX_LENGTH]);
#endif