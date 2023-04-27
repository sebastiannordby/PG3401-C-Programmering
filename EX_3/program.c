#include "flight-booking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    FlightList *flightList = malloc(sizeof(FlightList));
    if(flightList == NULL) {
        printf("Could not create flight list.\r\n");
        return 1;
    }

    Flight* f1 = addFlight(flightList, "S8993", "Oslo Lufthavn", 22, 1020);
    Flight* f2 = addFlight(flightList, "Z2938", "Gdansk", 22, 1020);
    Flight* f3 = addFlight(flightList, "G2938", "Alanya", 22, 1020);
    Flight* f4 = addFlight(flightList, "D2938", "Copenhagen", 22, 1020);

    printFlightList(flightList);

    printf("\r\nAdding passengers\r\n");

    addPassenger(f1, 1, "Ola Normann\0", 25);
    printf("Added first\n");
    addPassenger(f1, 2, "Heidi Olsen\0", 23);
    printf("Added second\n");

    printPassengerList(f1);

    return 0;
}