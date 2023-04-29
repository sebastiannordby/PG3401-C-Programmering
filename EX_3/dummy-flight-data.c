#include <stdio.h>
#include "flight-booking.h"

void usePredefinedDummyData(FlightList *pFlightList) {
    printf("Using predefined data..\r\n");

    printf("Adding flights...\r\n");

    char f1Id[FLIGHT_ID_MAX_LENGTH] = "OSL1234\0";
    char f1Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Oslo Lufthavn\0";
    char f2Id[FLIGHT_ID_MAX_LENGTH] = "GDN1234\0";
    char f2Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Gdansk Airport\0";
    char f3Id[FLIGHT_ID_MAX_LENGTH] = "AYA1234\0";
    char f3Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Alanya International\0";
    char f4Id[FLIGHT_ID_MAX_LENGTH] = "CHN1234\0";
    char f4Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Copenhagen\0";
    char f5Id[FLIGHT_ID_MAX_LENGTH] = "STC1234\0";
    char f5Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Stockholm Airport\0";
    char f6Id[FLIGHT_ID_MAX_LENGTH] = "TEN1234\0";
    char f6Dest[FLIGHT_DESTINATION_MAX_LENGTH] = "Tenerife Intr Airport\0";

    Flight* f1 = addFlight(pFlightList, f1Id, f1Dest, 22, 820);
    Flight* f2 = addFlight(pFlightList, f2Id, f2Dest, 22, 1020);
    Flight* f3 = addFlight(pFlightList, f3Id, f3Dest, 22, 1120);
    Flight* f4 = addFlight(pFlightList, f4Id, f4Dest, 22, 1220);
    Flight* f5 = addFlight(pFlightList, f5Id, f5Dest, 22, 1320);
    Flight* f6 = addFlight(pFlightList, f6Id, f6Dest, 22, 1420);

    printf("Adding passengers\r\n");

    char f1P1Name[PASSENGER_NAME_MAX_LENGTH] = "Dennis MacAlistair Ritchie";
    char f1P2Name[PASSENGER_NAME_MAX_LENGTH] = "Aaron Swartz";
    char f1P3Name[PASSENGER_NAME_MAX_LENGTH] = "Linus Thorvalds";
    addPassenger(f1, 0, f1P1Name, 55);
    addPassenger(f1, 0, f1P2Name, 23);
    addPassenger(f1, 0, f1P3Name, 56);

    char f2P1Name[PASSENGER_NAME_MAX_LENGTH] = "Alan Turing";
    char f2P2Name[PASSENGER_NAME_MAX_LENGTH] = "Bill Gates";
    char f2P3Name[PASSENGER_NAME_MAX_LENGTH] = "Linus Thorvalds";
    addPassenger(f2, 0, f2P1Name, 21);
    addPassenger(f2, 0, f2P2Name, 42);
    addPassenger(f2, 0, f2P3Name, 65);

    char f6P1Name[PASSENGER_NAME_MAX_LENGTH] = "Tim Berners-Lee";
    char f6P2Name[PASSENGER_NAME_MAX_LENGTH] = "James Gosling";
    char f6P3Name[PASSENGER_NAME_MAX_LENGTH] = "Brian Kernighan";
    addPassenger(f6, 1, f6P1Name, 21);
    addPassenger(f6, 2, f6P2Name, 42);
    addPassenger(f6, 3, f6P3Name, 88);

    printFlight(f6);
    printFlight(f6);

    printf("Program now uses predefined data.\r\n");
}
