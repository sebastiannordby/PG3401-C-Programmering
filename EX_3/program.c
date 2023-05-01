#include "flight-booking.h"
#include "dummy-flight-data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Menu choices
const unsigned short MN_ADD_FLIGHT = 1;
const unsigned short MN_ADD_PASSENGER = 2;
const unsigned short MN_PRINT_FLIGHT = 3;
const unsigned short MN_FIND_FLIGHT_BY_TIME = 4;
const unsigned short MN_FIND_FLIGHT_BY_INDEX = 5;
const unsigned short MN_DELETE_FLIGHT = 6;
const unsigned short MN_REMOVE_PASSENGER_RES = 7;
const unsigned short MN_CHANGE_PASSENGER_SEAT = 8;
const unsigned short MN_SEARCH_PASSENGER = 9;
const unsigned short MN_PRINT_ALL_FLIGHTS = 10;
const unsigned short MN_EXIT_PROGRAM = 11;

// Declare functions to skip precendence in code
int read_menu_choice(void);
void free_flight_list(FlightList *flight_list);
void read_string(char *str, int maxLength);
void print_menu(void);
void print_menu_header(char *header);
void print_failed_input(char *inputName);
void prompt_enter_input(char *inputName);
void prompt_add_flight(FlightList *flight_list);
void prompt_add_passenger(FlightList *flight_list);
void prompt_print_flight(FlightList *flight_list);
void prompt_find_flight_by_departure_time(FlightList *flight_list);
void prompt_find_flight_by_index(FlightList *flight_list);
void prompt_delete_flight(FlightList *flight_list);
void prompt_remove_passenger_res(FlightList *flight_list);
void prompt_change_passenger_seat(FlightList *flight_list);
void prompt_search_passenger(FlightList *flight_list);

int main(int argc, char *argv[]) {
    FlightList *flight_list = malloc(sizeof(FlightList));
    if(flight_list == NULL) {
        printf("Could not create flight list.\r\n");
        return 1;
    }

    // If program if runned with -predef run with preloaded
    // flight data. Easier for debugging purposes.
    if(argc == 2 && strcmp(argv[1], "-predef") == 0) 
        use_predefined_dummy_data(flight_list);

    // Handle the menu prompting and delegation of which
    // operation the user has choosen.
    int choice;

    while((choice = read_menu_choice()) != MN_EXIT_PROGRAM && choice != EOF) {
        switch(choice) {
            case MN_ADD_FLIGHT:
                prompt_add_flight(flight_list);
                break;
            case MN_ADD_PASSENGER:
                prompt_add_passenger(flight_list);
                break;
            case MN_PRINT_FLIGHT:
                prompt_print_flight(flight_list);
                break;
            case MN_FIND_FLIGHT_BY_TIME:
                prompt_find_flight_by_departure_time(flight_list);
                break;
            case MN_FIND_FLIGHT_BY_INDEX:
                prompt_find_flight_by_index(flight_list);
                break;
            case MN_DELETE_FLIGHT:
                prompt_delete_flight(flight_list);
                break;
            case MN_REMOVE_PASSENGER_RES:
                prompt_remove_passenger_res(flight_list);
                break;
            case MN_CHANGE_PASSENGER_SEAT:
                prompt_change_passenger_seat(flight_list);
                break;
            case MN_PRINT_ALL_FLIGHTS:
                print_flight_list(flight_list);
                break;
            case MN_SEARCH_PASSENGER:
                prompt_search_passenger(flight_list);
                break;
            default:
                printf("Unvalid menu choice.\r\n");
        }
    }

    free_flight_list(flight_list);
    printf("\r\nProgram exited. You can now close the terminal.\r\n");

    return EXIT_SUCCESS;
}

void print_menu(void) {
    printf("----------- Menu -----------\r\n");
    printf("%d. Add flight\r\n", MN_ADD_FLIGHT);
    printf("%d. Add passenger to a flight\r\n", MN_ADD_PASSENGER);
    printf("%d. Print flight\r\n", MN_PRINT_FLIGHT);
    printf("%d. Find flight by time\r\n", MN_FIND_FLIGHT_BY_TIME);
    printf("%d. Find flight by index\r\n", MN_FIND_FLIGHT_BY_INDEX);
    printf("%d. Delete a flight\r\n", MN_DELETE_FLIGHT);
    printf("%d. Remove passenger reservation\r\n", MN_REMOVE_PASSENGER_RES);
    printf("%d. Change seat for passenger\r\n", MN_CHANGE_PASSENGER_SEAT);
    printf("%d. Search for passenger\r\n", MN_SEARCH_PASSENGER);
    printf("%d. List all flights\r\n", MN_PRINT_ALL_FLIGHTS);
    printf("%d. Exit the program\r\n", MN_EXIT_PROGRAM);
}

int read_menu_choice() {
    print_menu();
    printf("booking> ");
    int choice;
    int scan_res = scanf("%d", &choice);
    getchar();

    return scan_res != EOF ? choice : EOF;
}

void print_menu_header(char *text) {
    printf("-------------------- %s --------------------\r\n", text);
}

void read_string(char *str, int max_length) {
    int current, counter = 0;

    while(counter < max_length && (current = getchar()) != '\n') {
        str[counter] = current;
        counter++;
    }

    str[counter] = '\0';
}

void prompt_enter_input(char *input_name) {
    printf("Enter %s: ", input_name);
}

void prompt_add_flight(FlightList *flight_list) {
    print_menu_header("ADD FLIGHT");

    char flight_id[FLIGHT_ID_MAX_LENGTH];
    prompt_enter_input("id");
    read_string(flight_id, FLIGHT_ID_MAX_LENGTH);

    char destination[FLIGHT_DESTINATION_MAX_LENGTH];
    prompt_enter_input("destination");
    read_string(destination, FLIGHT_DESTINATION_MAX_LENGTH);

    unsigned short number_of_seats;
    prompt_enter_input("number of seats");
    if(scanf("%hu", &number_of_seats) != 1) {
        print_failed_input("number of seats");
        return;
    }

    unsigned short departure_time;
    prompt_enter_input("departure time");
    if(scanf("%hu", &departure_time) != 1) {
        print_failed_input("departure time");
        return;
    }

    Flight *flight = add_flight(flight_list, flight_id, destination, number_of_seats, departure_time);

    if(flight != NULL) {
        printf("Successfully created flight \"%s\"\r\n", flight_id);
        printf("Here is the updated flight list:\r\n");
        print_flight_list(flight_list);
        printf("\r\n");
    }
}

void prompt_add_passenger(FlightList *flight_list) {
    print_menu_header("ADD PASSENGER TO A FLIGHT");

    char flight_id[FLIGHT_ID_MAX_LENGTH];
    prompt_enter_input("flight id");
    read_string(flight_id, FLIGHT_ID_MAX_LENGTH);

    Flight* flight = get_flight_by_id(flight_list, flight_id);
    if(flight == NULL) {
        printf("No flight with id of: %s\r\n", flight_id);
        return;
    }

    char name[PASSENGER_NAME_MAX_LENGTH];
    prompt_enter_input("name");
    read_string(name, PASSENGER_NAME_MAX_LENGTH);

    unsigned short age;
    prompt_enter_input("age");
    if(scanf("%hu", &age) != 1) {
        print_failed_input("age");
        return;
    }

    unsigned short seat_number;
    prompt_enter_input("seat number(-1 for automatic allocation)");
    if(scanf("%hu", &seat_number) != 1) {
        print_failed_input("seatnumber");
        return;
    }

    if(add_passenger(flight, seat_number, name, age)) {
        printf("Successfully added passenger \"%s\" to flight \"%s\"\r\n", 
            flight_id, name);
    } else {
        printf("Could not add passenger to flight.\r\n");
    }
}

void free_flight_list(FlightList *flight_list) {
    Flight *flight = flight_list->head;

    while (flight != NULL) {
        Passenger *pPassenger = flight->passengers;

        while (pPassenger != NULL) {
            Passenger *pPassengerTemp = pPassenger;
            pPassenger = pPassenger->next;
            free(pPassengerTemp);
        }
    
        Flight *pTempFlight = flight;
        flight = flight->next;
        free(pTempFlight);
    }

    free(flight_list);
    flight_list = NULL;
}

void prompt_print_flight(FlightList *flight_list) {
    print_menu_header("PRINT FLIGHT");
    char flight_id[FLIGHT_ID_MAX_LENGTH];
    prompt_enter_input("flight id");
    read_string(flight_id, FLIGHT_ID_MAX_LENGTH);

    Flight* flight = get_flight_by_id(flight_list, flight_id);
    if(flight == NULL) {
        printf("No flight with id of: %s\r\n", flight_id);
        return;
    }

    printf("Information for flight %s\r\n", flight_id);
    print_flight(flight);
}

void prompt_find_flight_by_index(FlightList *flight_list) {
    print_menu_header("FIND FLIGHT BY INDEX");
    prompt_enter_input("index");
    unsigned short index;
    if(scanf("%hu", &index) != 1) {
        print_failed_input("index");
        return;
    }

    Flight *flight = get_flight_by_index(flight_list, index);

    if(flight == NULL) 
        return;

    print_flight(flight);
}

void prompt_find_flight_by_departure_time(FlightList *flight_list) {
    print_menu_header("FIND FLIGHT BY DEPARTURE TIME");
    prompt_enter_input("departure");
    unsigned short departure_time;
    if(scanf("%hu", &departure_time) != 1) {
        print_failed_input("departure time");
        return;
    }

    Flight *flight = get_flight_by_departure_time(flight_list, departure_time);

    if(flight == NULL) {
        printf("Could not find flight with depature at \"%hu\"\r\n", departure_time);
        return;
    }

    printf("Found flight with depature at \"%hu\"\r\n", departure_time);
    print_flight(flight);
}

void prompt_delete_flight(FlightList *flight_list) {
    print_menu_header("DELETE FLIGHT");
    char flight_id[FLIGHT_ID_MAX_LENGTH];
    prompt_enter_input("flight id");
    read_string(flight_id, FLIGHT_ID_MAX_LENGTH);

    if(delete_flight(flight_list, flight_id)) 
        printf("Successfully deleted flight \"%s\"\r\n", flight_id);
    else 
        printf("Failed to delete flight with id of: \"%s\"\r\n", flight_id);
}

void prompt_remove_passenger_res(FlightList *flight_list) {
    print_menu_header("REMOVE PASSENGER RESERVATION");

    char flight_id[FLIGHT_ID_MAX_LENGTH];
    prompt_enter_input("flight id");
    read_string(flight_id, FLIGHT_ID_MAX_LENGTH);

    unsigned short seat_number;
    printf("Enter seat number: ");
    if(scanf("%hu", &seat_number) != 1) {
        print_failed_input("seatnumber");
        return;
    }

    if(remove_passenger(flight_list, flight_id, seat_number)) 
        printf("Successfully removed passenger reservation.\r\n");
    else 
        printf("Failed to delete passenger reservation.\r\n");
}

void print_failed_input(char *input_name) {
    printf("An error occured inputing %s\r\n", input_name);
}

void prompt_change_passenger_seat(FlightList *flight_list) {
    print_menu_header("CHANGE PASSENGER SEAT");

    char flight_id[FLIGHT_ID_MAX_LENGTH];
    prompt_enter_input("flight id");
    read_string(flight_id, FLIGHT_ID_MAX_LENGTH);

    printf("Enter current seat number: ");
    unsigned short current_seat_number;
    if(scanf("%hu", &current_seat_number) != 1) {
        print_failed_input("current seat number");
        return;
    }

    prompt_enter_input("requested seat number");
    unsigned short requested_seat_number;
    if(scanf("%hu", &requested_seat_number) != 1) {
        print_failed_input("current seat number");
        return;
    }

    if(change_passenger_seat(flight_list, flight_id, current_seat_number, requested_seat_number)) 
        printf("Successfully changed seat from \"%hu\" to \"%hu\"\r\n",
            current_seat_number, requested_seat_number);
    else 
        printf("Unable to change seat from \"%hu\" to \"%hu\"\r\n", 
            current_seat_number, requested_seat_number);
}

void prompt_search_passenger(FlightList *flight_list) {
    print_menu_header("SEARCH PASSENGER FLIGHTS");
    char passengerName[PASSENGER_NAME_MAX_LENGTH];
    prompt_enter_input("name");
    read_string(passengerName, PASSENGER_NAME_MAX_LENGTH);

    unsigned int numberOfFlights = 
        get_associated_flights_by_passenger_name(flight_list, passengerName);

    printf("Number of flights accosiated with name: %s is %hu\r\n", passengerName, numberOfFlights);
}

