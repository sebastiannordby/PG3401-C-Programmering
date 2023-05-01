#include <stdio.h>
#include "flight-booking.h"

void use_predefined_dummy_data(FlightList *pFlightList) {
    printf("Using predefined data..\r\n");

    printf("Adding flights...\r\n");

    char f1Id[FLIGHT_ID_MAX_LENGTH] = "OSL1234\0";
    char f1Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Oslo Airport (OSL)\0";
    char f2Id[FLIGHT_ID_MAX_LENGTH] = "GDN1234\0";
    char f2Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Gdansk Airport\0";
    char f3Id[FLIGHT_ID_MAX_LENGTH] = "AYA1234\0";
    char f3Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Alanya International\0";
    char f4Id[FLIGHT_ID_MAX_LENGTH] = "CHN1234\0";
    char f4Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Copenhagen Airport\0";
    char f5Id[FLIGHT_ID_MAX_LENGTH] = "STC1234\0";
    char f5Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Stockholm Airport\0";
    char f6Id[FLIGHT_ID_MAX_LENGTH] = "TEN1234\0";
    char f6Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Tenerife Intr Airport\0";

    Flight* f1 = add_flight(pFlightList, f1Id, f1Dest, 22, 820);
    Flight* f2 = add_flight(pFlightList, f2Id, f2Dest, 22, 1020);
    Flight* f3 = add_flight(pFlightList, f3Id, f3Dest, 22, 1120);
    Flight* f4 = add_flight(pFlightList, f4Id, f4Dest, 22, 1220);
    Flight* f5 = add_flight(pFlightList, f5Id, f5Dest, 22, 1320);
    Flight* f6 = add_flight(pFlightList, f6Id, f6Dest, 22, 1420);

    printf("Adding passengers\r\n");

    char f1_p1_name[PASSENGER_NAME_MAX_LENGTH] = "Dennis MacAlistair Ritchie";
    char f1_p2_name[PASSENGER_NAME_MAX_LENGTH] = "Aaron Swartz";
    char f1_p3_name[PASSENGER_NAME_MAX_LENGTH] = "Linus Thorvalds";
    add_passenger(f1, 0, f1_p1_name, 55);
    add_passenger(f1, 0, f1_p2_name, 23);
    add_passenger(f1, 0, f1_p3_name, 56);

    char f2_p1_name[PASSENGER_NAME_MAX_LENGTH] = "Alan Turing";
    char f2_p2_name[PASSENGER_NAME_MAX_LENGTH] = "Bill Gates";
    char f2_p3_name[PASSENGER_NAME_MAX_LENGTH] = "Linus Thorvalds";
    add_passenger(f2, 0, f2_p1_name, 21);
    add_passenger(f2, 0, f2_p2_name, 42);
    add_passenger(f2, 0, f2_p3_name, 65);

    char f6_p1_name[PASSENGER_NAME_MAX_LENGTH] = "Tim Berners-Lee";
    char f6_p2_name[PASSENGER_NAME_MAX_LENGTH] = "James Gosling";
    char f6_p3_name[PASSENGER_NAME_MAX_LENGTH] = "Brian Kernighan";
    add_passenger(f6, 1, f6_p1_name, 21);
    add_passenger(f6, 2, f6_p2_name, 42);
    add_passenger(f6, 3, f6_p3_name, 88);

    
    print_flight(f1);
    print_flight(f2);
    print_flight(f3);
    print_flight(f4);
    print_flight(f5);
    print_flight(f6);

    printf("Program now uses predefined data as displayed above.\r\n");
}
