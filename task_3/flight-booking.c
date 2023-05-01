#include "include/flight-booking.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// Declare functions to skip precendence in code
void print_dotted_line(int length);
bool is_seat_taken(Passenger *p, unsigned short seat_number);

/*
    Prints out a flight in the following format:
    {id} | {destination} | {departure}

    Then the passenger list in the following format:
    {seat} | {name} | {age}
*/
void print_flight(const Flight *flight) {
    print_dotted_line(100);
    printf("%-10s| %-50s| %-10s | %-10s\r\n", 
        "Id", "Destination", "Depature", "Seats");
    printf("%-10s| %-50s| %-10d | %-10d\r\n", 
        flight->flightID, flight->destination, 
        flight->departure_time, flight->number_of_seats);

    
    if(flight->passengers != NULL) {
        printf("\r\nPassengers for flight %s:\r\n", flight->destination);
        print_dotted_line(100);

        printf("%-5s| %-50s| %-3s\r\n", 
            "Seat", "Name", "Age");

        Passenger *currentPassenger = flight->passengers;

        // Loop through all passengers on the flight.
        while(currentPassenger != NULL) {
            printf("%-5hu| %-50s| %-3hu\r\n",
                currentPassenger->seat_number,
                currentPassenger->name,
                currentPassenger->age);

            currentPassenger = currentPassenger->next;
        }
    } else {
        printf("\r\nNo passengers registred for flight %s:\r\n", flight->destination);
    }

    print_dotted_line(100);
}

// Get flights associated with passenger name.
unsigned int get_associated_flights_by_passenger_name(const FlightList *flight_list, 
    char passenger_name[PASSENGER_NAME_MAX_LENGTH]) {

    if(flight_list == NULL || passenger_name == NULL || flight_list->head == NULL)
        return 0;

    unsigned int numberOfAssociatedFlights = 0;
    Flight* currentFlight = flight_list->head;

    // Loop through all flight.
    while (currentFlight != NULL) {
        Passenger* currentPassenger = currentFlight->passengers;
        
        // Loop through all passengers on the current flight.
        while (currentPassenger != NULL) {
             // If there is a person with the given name on the flight,
             // there is no need to check remaining passengers
            if (strcasecmp(currentPassenger->name, passenger_name) == 0) {
                numberOfAssociatedFlights++;
                break;
            }

            currentPassenger = currentPassenger->next;
        }

        currentFlight = currentFlight->next;
    }

    return numberOfAssociatedFlights;
}

// Prints out a dotted line in the provided length.
void print_dotted_line(int length) {
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
void print_flight_list(const FlightList* flight_list) {
    if(flight_list == NULL || flight_list->head == NULL){
        printf("List of flight is empty.\r\n");
        return;
    }

    int num_of_dots = 82;
    print_dotted_line(num_of_dots);
    printf("%-10s| %-50s| %-10s | %-10s\r\n", "Id", "Destination", "Depature", "Seats");
    print_dotted_line(num_of_dots);

    Flight *currentFlight = flight_list->head;

    // Loop through all flights.
    while (currentFlight != NULL) {
        printf("%-10s| %-50s| %-10d | %-10d\r\n", 
            currentFlight->flightID, currentFlight->destination, 
            currentFlight->departure_time, currentFlight->number_of_seats);
        currentFlight = currentFlight->next;
    }

    print_dotted_line(num_of_dots);
}

// Adds a flight to the flight list.
Flight* add_flight(
    FlightList* flight_list, 
    char flight_id[FLIGHT_ID_MAX_LENGTH], 
    char destination[FLIGHT_DESTINATION_MAX_LENGTH], 
    unsigned short number_of_seats, 
    unsigned short departure_time) {

    // Create the flight struct    
    Flight* newFlight = malloc(sizeof(Flight));
    strcpy(newFlight->flightID, flight_id);
    strcpy(newFlight->destination, destination);
    newFlight->number_of_seats = number_of_seats;
    newFlight->departure_time = departure_time;
    newFlight->next = NULL;
    newFlight->prev = NULL;
    newFlight->passengers = NULL;

    // If there is no elements, head is null
    // then we can assign the head to be the created flight
    if (flight_list->head == NULL) {
        flight_list->head = newFlight;
        flight_list->tail = newFlight;
    } else {
        flight_list->tail->next = newFlight;
        newFlight->prev = flight_list->tail;
        flight_list->tail = newFlight;
    }

    return newFlight;
}

// Delete a flight by id.
bool delete_flight(FlightList *flight_list, char flight_id[FLIGHT_ID_MAX_LENGTH]) {
    if(flight_list == NULL || flight_list->head == NULL)
        return false;

    Flight *flight = flight_list->head;

    while (flight != NULL) {
        if(strcasecmp(flight->flightID, flight_id) == 0) {
            // If the first flight is removed  
            // Set the next flight to be the head
            if(flight == flight_list->head) {
                flight_list->head = flight->next;
            } 
            
            // If the last flight is removed
            // Set the previous flight to be the tail
            if(flight == flight_list->tail) {
                flight_list->tail = flight->prev;
            } 

            // If there is a flight before the target in the list:
            // Sets it's previous to be the targeteds nexts.
            if(flight->prev != NULL) {
                flight->prev->next = flight->next;
            }

            // If there is a flight after the targeted in the list:
            // Set it's next to be the targeteds flights previous.
            if(flight->next != NULL) {
                flight->next->prev = flight->prev;
            }

            Passenger *current_passenger = flight->passengers;

            // Loop through all passengers to free them from memory.
            while (current_passenger != NULL) {
                Passenger *pPassengerTemp = current_passenger;
                current_passenger = current_passenger->next;
                free(pPassengerTemp);
            }

            free(flight);
            flight = NULL;

            return true;
        }

        flight = flight->next;
    }
    
    return false;
}

// Checks if seat number is taken.
bool is_seat_taken(Passenger *p, unsigned short seat_number) {
    if(p == NULL)
        return false;

    if(p->seat_number == seat_number)
        return true;

    if(p->next != NULL && p->next->seat_number == seat_number)
        return true;

    return false;
}

/*
    Returns the passenger closest to the seat number provided.
*/
Passenger* get_passenger_closest_to_seat_number(Flight *flight, unsigned short seatNumber) {
    Passenger *current_passenger = flight->passengers;

    while(current_passenger != NULL && 
        current_passenger->seat_number < seatNumber && 
        current_passenger->next != NULL) {
        current_passenger = current_passenger->next;
    }

    return current_passenger;
}

/*
    Adds a passenger to the given flight.
    If the requstedSeatNumber is 0, take the first available.
*/
bool add_passenger(Flight *flight, unsigned short requested_seat_number, 
    char name[PASSENGER_NAME_MAX_LENGTH], unsigned short age) {
    
    // Check if arguments passed is valid
    if(flight == NULL || name == NULL)
        return false;

    Passenger *new_passenger = malloc(sizeof(Passenger));
    new_passenger->age = age;
    new_passenger->seat_number = requested_seat_number != 0 ? requested_seat_number : 1;
    strcpy(new_passenger->name, name);
    new_passenger->name[PASSENGER_NAME_MAX_LENGTH - 1] = '\0';

    if(flight->passengers == NULL) {
        flight->passengers = new_passenger;
    } else if(requested_seat_number > 0) {
        Passenger *closestPassenger = get_passenger_closest_to_seat_number(
            flight, requested_seat_number);

        // Tried to use an existing seatnumber
        if(is_seat_taken(closestPassenger, requested_seat_number)) {
            printf("Seat: %d is already taken.\r\n", requested_seat_number);
            free(new_passenger);
            new_passenger = NULL;

            return false;
        }

        new_passenger->next = closestPassenger->next;
        closestPassenger->next = new_passenger;
    } else {
        Passenger *currentPassenger = flight->passengers;

        while(currentPassenger != NULL && 
            currentPassenger->next != NULL) {
            currentPassenger = currentPassenger->next;
        }

        if(currentPassenger->next == NULL && 
            currentPassenger->seat_number + 1 < flight->number_of_seats) {
            new_passenger->seat_number = currentPassenger->seat_number + 1;
        }

        new_passenger->next = currentPassenger->next;
        currentPassenger->next = new_passenger;
    }

    return true;
}

// Removes a passenger from a given flight.
bool remove_passenger(
    FlightList *flight_list,
    char flight_id[FLIGHT_ID_MAX_LENGTH], unsigned short seat_number) {
    if(flight_id == NULL || seat_number == 0)
        return false;

    Flight *flight = get_flight_by_id(flight_list, flight_id);
    if(flight == NULL)
        return false;

    Passenger *current_passenger = flight->passengers;
    Passenger *previous_passenger = NULL;

    // Loop through the passenger list.
    while(current_passenger != NULL) {
        // Check if it's the targeted passenger.
        if(current_passenger->seat_number == seat_number) {
            if(previous_passenger != NULL) {
                previous_passenger->next = current_passenger->next;
            }

            free(current_passenger);
            current_passenger = NULL;

            return true;
        }

        previous_passenger = current_passenger;
        current_passenger = current_passenger->next;
    }

    return false;
}

// Change passenger seat number.
bool change_passenger_seat(FlightList *flight_list, char flight_id[FLIGHT_ID_MAX_LENGTH],
    unsigned short current_seat_number, unsigned short requested_seat_number) {

    if(flight_list == NULL || flight_id == NULL)
        return false;

    Flight *flight = get_flight_by_id(flight_list, flight_id);
    if(flight == NULL)
        return false;

    Passenger *passenger_list = flight->passengers;

    // Find the passenger with the current seat number
    Passenger *current_passenger = passenger_list;
    while (current_passenger != NULL && current_passenger->seat_number != current_seat_number) {
        current_passenger = current_passenger->next;
    }
    
    if (current_passenger == NULL) 
        return false;
    
    // Check if the requested seat number is already taken
    Passenger *temp_passenger = passenger_list;
    while (temp_passenger != NULL && temp_passenger->seat_number != requested_seat_number) {
        temp_passenger = temp_passenger->next;
    }
    if (temp_passenger != NULL) {
        return false;
    }
    
    // Update the seat number of the targeted passenger
    current_passenger->seat_number = requested_seat_number;
    
    // Remove the targeted passenger from the list
    if (current_passenger == passenger_list) {
        passenger_list = current_passenger->next;
    } else {
        Passenger *prev_passenger = passenger_list;
        while (prev_passenger->next != current_passenger) {
            prev_passenger = prev_passenger->next;
        }

        prev_passenger->next = current_passenger->next;
    }
    
    // Re-insert the targeted passenger into the list at the correct position
    Passenger *prevPassenger = NULL;
    temp_passenger = passenger_list;
    while (temp_passenger != NULL && temp_passenger->seat_number < requested_seat_number) {
        prevPassenger = temp_passenger;
        temp_passenger = temp_passenger->next;
    }
    if (prevPassenger == NULL) {
        current_passenger->next = passenger_list;
        passenger_list = current_passenger;
    } else {
        current_passenger->next = prevPassenger->next;
        prevPassenger->next = current_passenger;
    }

    flight->passengers = passenger_list;
    
    return true;
}

void print_passenger_list(const Flight *flight) {
    if(flight == NULL || flight->passengers == NULL)
        return;

    Passenger *current_passenger = flight->passengers;

    while(current_passenger != NULL) {
        printf("Seat: %d\tName: %s\r\n", 
            current_passenger->seat_number,
            current_passenger->name);

        current_passenger = current_passenger->next;
    }
}

// Returns a flight by the given flightId. 
// Ignores case sensitivity.
Flight* get_flight_by_id(FlightList *flight_list, char flight_id[FLIGHT_ID_MAX_LENGTH]) {
    if(flight_list == NULL || flight_id == NULL)
        return NULL;

    Flight *current_flight = flight_list->head;

    // Loops until the flight is found, or 
    // an occurence of NULL which suggest that there is no flight
    // with the provided id.
    while(current_flight != NULL && strcasecmp(current_flight->flightID, flight_id) != 0) {
        current_flight = current_flight->next;
    }

    return current_flight;
}

/*
    Returns a flight at the given index. 
    Indexing starts as 1.
*/
Flight* get_flight_by_index(FlightList *flight_list, unsigned short index) {
    if(flight_list == NULL)
        return NULL;
    
    int currIndex = 1;
    Flight *currentFlight = flight_list->head;

    while(currentFlight != NULL && currIndex <= index - 1) {
        currentFlight = currentFlight->next;
        currIndex++;
    }

    return currentFlight;
}

Flight* get_flight_by_departure_time(FlightList *flight_list, unsigned short time) {
    if(flight_list == NULL)
        return NULL;

    Flight *current_flight = flight_list->head;

    while(current_flight != NULL && current_flight->departure_time != time) {
        current_flight = current_flight->next;
    }

    return current_flight;
}
