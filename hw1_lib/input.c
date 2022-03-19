#include "input.h"
#include "utils.h"

int one_station(FILE *fin, FILE *fout, int station_number, int train_number) {
  char name[MAX_CHARS];
  cell train = {0};
  train.station_number = station_number;
  train.train_number = train_number;
  Station station = {0};
  station.trains_capacity = 0;
  station.trains_size = 0;
  station.allTrains = NULL;
  fprintf(fout, "Введите название станции (не более 49 символов)\n");
  int args_read = fscanf(fin, "%49s", name);
  // предусмотреть вычитку буфера? до \n
  if (args_read != 1) {
    return 1;
  }
  station.Name = malloc(strlen(name) + 1);
  strncpy(station.Name, name, strlen(name) + 1);

  train.arrivTime.hour = -1;
  train.arrivTime.min = -1;  // для станции отправления

  char end_flag = 0;
  if (train.station_number != 0) {  // sне станция отправления
    fprintf(fout, "Введите время прибытия в формате чч:мм\n");
    args_read =
        fscanf(fin, "%d :%d", &(train.arrivTime.hour), &(train.arrivTime.min));
    if (args_read != 2 || !time_validation(train.arrivTime)) {
      fprintf(fout, "\nНеправильный формат времени или ошибка ввода\n");
      free(station.Name);
      return 1;
    }

    fprintf(fout, "часы = %d, минуты = %d\n", (train.arrivTime.hour),
            (train.arrivTime.min));
    fprintf(fout, "Это конечная? Ответьте y, если да, другую букву - нет\n");
    fscanf(fin, "%*[^\n]");
    args_read = fscanf(fin, "\n%c", &end_flag);
    if (args_read != 1) {
      free(station.Name);
      return 1;
    }
  }

  if (end_flag == YES) {
    train.departTime.hour = -1;
    train.departTime.min = -1;
    add_station(station, train);
    return 0;
  }

  fprintf(fout, "Введите время отправления в формате чч:мм\n");
  args_read =
      fscanf(fin, "%d :%d", &(train.departTime.hour), &(train.departTime.min));
  if (args_read != 2 || !time_validation(train.departTime)) {
    free(station.Name);
    fprintf(fout, "\nНеправильный формат времени или ошибка ввода\n");
    return 1;
  }
  fprintf(fout, "часы = %d, минуты = %d\n", (train.departTime.hour),
          (train.departTime.min));
  add_station(station, train);
  return 1;
}

void one_train(FILE *fin, FILE *fout, int trainnumber) {
  int a = 1;
  int station_number = 0;
  while (a) {
    a = one_station(fin, fout, station_number, trainnumber);
    station_number++;
  }
}

int all_trains(FILE *fin, FILE *fout) {
  int trainnumber = 0;
  char last = 0;
  while (last != YES) {
    fprintf(fout, "Введите новый поезд\n");
    one_train(fin, fout, trainnumber);
    fprintf(
        fout,
        "Это последняя электричка? Ответьте y, если да, другую букву - нет\n");
    fscanf(fin, "%*[^\n]");
    int args = fscanf(fin, "\n%c", &last);
    fscanf(fin, "%*[^\n]");
    if (args != 1) {
      return -1;
    }
    trainnumber++;
  }
  return 0;
}

int find_nearest(FILE *fin, FILE *fout) {
  char first_station[MAX_CHARS];
  fprintf(fout, "Ищем поезд\n");
  fprintf(fout,
          "Введите название станции отправления (не более 49 символов)\n");
  int args_read = fscanf(fin, "%49s", first_station);
  if (args_read != 1) {
    return 1;
  }
  char last_station[MAX_CHARS];
  fprintf(fout, "Введите название станции прибытия (не более 49 символов)\n");
  args_read = fscanf(fin, "%49s", last_station);
  if (args_read != 1) {
    return 1;
  }
  time_24 now;
  fprintf(fout, "Введите текущее время в формате чч:мм\n");
  args_read = fscanf(fin, "%d :%d", &now.hour, &now.min);
  if (args_read != 2 || !time_validation(now)) {
    fprintf(fout, "\nНеправильный формат времени или ошибка ввода\n");
    return 1;
  }
  cell train = find_train(first_station, last_station, &now);
  if (train.departTime.hour != -1) {
    fprintf(fout, "Следующий поезд отправится в %d часов, %d минут\n",
            (train.departTime.hour), (train.departTime.min));
    return 0;
  }
  fprintf(fout, "Таких поездов нет");
  return 1;
}
