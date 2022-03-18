#ifndef HW1_GENERAL_H
#define HW1_GENERAL_H
#include "types.h"
int find_insertion_point(const void *arr, int size, const void *element,
                         int (*compare)(const void *arr, const void *element, int number));
void *grow(void *buffer, int *current_capacity, size_t element_size);
int add_if_not_exists(void **buffer, int *capacity, int *size, void *element, size_t element_size,
                      int (*compare)(const void *arr, const void *element, int number),
                      void (*setter)(void **buffer, int pos, void *element), void *(*getter)(void **buffer, int pos));
#endif //HW1_GENERAL_H
