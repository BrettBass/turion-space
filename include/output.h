#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include "shared_memory.h"

#define MAX_BAR_LENGTH 50  // Max number of characters for the bar (you can adjust this)
#define MAX_SENSOR_VALUE 100.0  // The max value to scale sensor data to

void print_avg(float average, const int window_size, int index);

#endif // OUTPUT_H
