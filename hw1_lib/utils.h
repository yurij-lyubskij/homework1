#ifndef HW1_UTILS_H
#define HW1_UTILS_H
#include "types.h"
void cellSetter(void **buffer, int pos, void *element);
void *cellGetter(void **buffer, int pos);
int compare_trains(const void *arr, const void *element, int number);
int compare_time(const time_24 *el1, const time_24 *el2);
int compare_by_time(const void *arr, const void *element, int number);
int add_train(cell **trains_buff, int *trains_capacity, int *trains_size, cell element);
int add_by_time(cell **trains_buff, int *trains_capacity, int *trains_size, cell element);
void station_setter(void **buffer, int pos, void *element);
void *station_getter(void **buffer, int pos);
int compare_stations(const void *arr, const void *element, int number);
int add_station(Station new_station, cell new_train);
cell find_train(char *src, char *dest, time_24 *time_now);
char time_validation(time_24 t);
void free_stations(Station* array, int size);
void free_all();
void initialize();
#endif //HW1_UTILS_H
