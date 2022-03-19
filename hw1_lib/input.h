#ifndef HW1_INPUT_H
#define HW1_INPUT_H
#include "types.h"
int one_station(FILE *fin, FILE *fout, int station_number, int train_number);
void one_train(FILE *fin, FILE *fout, int trainnumber);
int all_trains(FILE *fin, FILE *fout);
int find_nearest(FILE *fin, FILE *fout);
void free_stations(Station* array, int size);
void free_all();
#endif //HW1_INPUT_H
