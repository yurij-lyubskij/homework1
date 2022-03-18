#ifndef HW1_TYPES_H
#define HW1_TYPES_H
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 50
#define MINS 59
#define HOURS 23

#define YES 'y'

typedef struct time_24{
    int32_t hour;
    int32_t min;
}time_24; //24-hour format

typedef struct cell {
    int train_number;
    time_24 arrivTime;
    time_24 departTime;
    int station_number;
}cell;

typedef struct Station {
    char *Name;
    cell *allTrains;
    int trains_size;
    int trains_capacity;
}Station;
#endif //HW1_TYPES_H
