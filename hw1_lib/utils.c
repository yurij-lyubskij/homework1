#include "utils.h"
#include "general.h"

//Общий Буфер
Station *stations_buff = NULL; // Буфер.
int stations_capacity = 0;     // Размер буфера.
int stations_size = 0; // Количество элементов в массиве.

// Буфер всех электричек, проходящих через обе станции
cell *trains_all = NULL; // Буфер.
int all_capacity = 0;    // Размер буфера.
int all_size = 0; // Количество элементов в массиве.

// Буфер электричек, проходящих через обе станции после текущего времени
cell *trains_later = NULL; // Буфер.
int later_capacity = 0;    // Размер буфера.
int later_size = 0; // Количество элементов в массиве.

void initialize() {
  stations_buff = NULL;  // Буфер.
  stations_capacity = 0; // Размер буфера.
  stations_size = 0; // Количество элементов в массиве.

  trains_all = NULL; // Буфер.
  all_capacity = 0;  // Размер буфера.
  all_size = 0; // Количество элементов в массиве.

  trains_later = NULL; // Буфер.
  later_capacity = 0;  // Размер буфера.
  later_size = 0; // Количество элементов в массиве.
}

void cellSetter(void **buffer, int pos, void *element) {
  cell **buff = (cell **)buffer;
  cell *elem = element;
  (*buff)[pos] = (*elem);
}

void *cellGetter(void **buffer, int pos) {
  cell **buff = (cell **)buffer;
  return (void *)((*buff) + pos);
}

int compare_trains(const void *arr, const void *element, int number) {
  if (((cell *)arr)[number].train_number < ((cell *)element)->train_number) {
    return -1;
  }
  if (((cell *)arr)[number].train_number == ((cell *)element)->train_number) {
    return 0;
  }
  return 1;
}

int compare_time(const time_24 *el1, const time_24 *el2) {
  if (el1->hour < el2->hour) {
    return -1;
  }
  if ((el1->hour == el2->hour) && (el1->min < el2->min)) {
    return -1;
  }
  if ((el1->hour == el2->hour) && (el1->min == el2->min)) {
    return 0;
  }
  return 1;
}

int compare_by_time(const void *arr, const void *element, int number) {
  return compare_time(&((cell *)arr)[number].departTime,
                      &((cell *)element)->departTime);
}

int add_train(cell **trains_buff, int *trains_capacity, int *trains_size,
              cell element) {
  return add_if_not_exists((void **)trains_buff, trains_capacity, trains_size,
                           &element, sizeof(cell), &compare_trains, &cellSetter,
                           &cellGetter);
}

int add_by_time(cell **trains_buff, int *trains_capacity, int *trains_size,
                cell element) {
  return add_if_not_exists((void **)trains_buff, trains_capacity, trains_size,
                           &element, sizeof(cell), &compare_by_time,
                           &cellSetter, &cellGetter);
}

// Stations
void station_setter(void **buffer, int pos, void *element) {
  Station **buff = (Station **)buffer;
  Station *elem = element;
  (*buff)[pos] = (*elem);
}

void *station_getter(void **buffer, int pos) {
  Station **buff = (Station **)buffer;
  return (void *)((*buff) + pos);
}

int compare_stations(const void *arr, const void *element, int number) {
  return (strcmp(((Station *)arr)[number].Name, ((Station *)element)->Name));
}

int add_station(Station new_station, cell new_train) {
  int before = stations_size;
  int pos = add_if_not_exists(
      (void **)&stations_buff, &stations_capacity, &stations_size, &new_station,
      sizeof(Station), &compare_stations, &station_setter, &station_getter);
  int after = stations_size;
  if (after == before) { //станция уже была
    free(new_station.Name);
  }
  Station *station = (stations_buff + pos);
  add_train(&(station->allTrains), &(station->trains_capacity),
            &(station->trains_size), new_train);
  return 0;
}

cell find_train(char *src, char *dest, time_24 *time_now) {
  cell retval = {0};
  retval.departTime.hour = -1;
  retval.departTime.min = -1;
  Station temp = {0};
  temp.Name = src;
  if (src == NULL || dest == NULL) {
    return retval;
  }
  int pos_src = find_insertion_point(stations_buff, stations_size - 1, &temp,
                                     compare_stations);
  if (pos_src < 0 || pos_src >= stations_size) {
    return retval; // station does not exist
  }
  if (compare_stations(stations_buff, &temp, pos_src) != 0) {
    return retval; // station does not exist
  }
  temp.Name = dest;
  int pos_dest = find_insertion_point(stations_buff, stations_size, &temp,
                                      compare_stations);
  if (pos_dest < 0 || pos_dest >= stations_size) {
    return retval; // station does not exist
  }
  if (compare_stations(stations_buff, &temp, pos_dest) != 0) {
    return retval; // station does not exist
  }
  cell *src_buff = stations_buff[pos_src].allTrains;
  cell *dst_buff = stations_buff[pos_dest].allTrains;
  int src_size = stations_buff[pos_src].trains_size;
  int dst_size = stations_buff[pos_dest].trains_size;
  int i = 0;
  int j = 0;
  for (j = 0; j < src_size; j++) {
    while (i < dst_size &&
           dst_buff[i].train_number < src_buff[j].train_number) {
      i++;
    }
    if (i == dst_size) {
      continue;
    }
    if (dst_buff[i].train_number == src_buff[j].train_number &&
        dst_buff[i].station_number > src_buff[j].station_number) {
      add_by_time(&trains_all, &all_capacity, &all_size, src_buff[j]);
      if (compare_time(time_now, &src_buff[j].departTime) < 0) {
        add_by_time(&trains_later, &later_capacity, &later_size, src_buff[j]);
      }
    }
  }

  if (all_size > 0) {
    retval = trains_all[0];
    free(trains_all);
  }

  if (later_size > 0) {
    retval = trains_later[0];
    free(trains_later);
  }
  return retval;
}

char time_validation(time_24 t) {
  if (t.hour < 0 || t.min < 0 || t.hour > HOURS || t.min > MINS) {
    return 0;
  }
  return 1;
}

void free_stations(Station *array, int size) {
  for (int i = 0; i < size; ++i) {
    free(array[i].Name);
    free(array[i].allTrains);
  }
  free(array);
}

void free_all() { free_stations(stations_buff, stations_size); }
