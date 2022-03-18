#include <gtest/gtest.h>

extern "C" {
#include "input.h"
#include "utils.h"
}

#define MAX_LEN 3000

TEST(timetable, TestBasics) {
    initialize();
    Station new_station = {nullptr};
    cell new_train = {0};
    EXPECT_EQ(0, add_station(new_station, new_train));
    free_all();
}

TEST(timetable, TestFull) {
    char input[MAX_LEN] = "Марьино\n11:40\nАндреевка\n12:20\ny\ny\nМарьино\nАндреевка\n11:10\n";
    char output[MAX_LEN];
    char etalon[MAX_LEN] = \
"Введите новый поезд\n\
Введите название станции (не более 49 символов)\n\
Введите время отправления в формате чч:мм\n\
часы = 11, минуты = 40\n\
Введите название станции (не более 49 символов)\n\
Введите время прибытия в формате чч:мм\n\
часы = 12, минуты = 20\n\
Это конечная? Ответьте y, если да, другую букву - нет\n\
Это последняя электричка? Ответьте y, если да, другую букву - нет\n\
Ищем поезд\n\
Введите название станции отправления (не более 49 символов)\n\
Введите название станции прибытия (не более 49 символов)\n\
Введите текущее время в формате чч:мм\n\
Следующий поезд отправится в 11 часов, 40 минут\n";

    FILE *instream = fmemopen(input, MAX_LEN, "r");
    FILE *outstream = fmemopen(output, MAX_LEN, "w");
    initialize();
    all_trains(instream, outstream);
    find_nearest(instream, outstream);
    fclose(instream);
    fclose(outstream);
    free_all();
    EXPECT_STRCASEEQ(output, etalon);
}


