#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include "shared_memory.h"
#include "output.h"


extern shared_memory_t shared_memory;

typedef struct {
    bool buffer_full; // Buffer full flags for each sensor
    int read_index;   // Read indices for each sensor
    float average;   // Average values for each sensor
} moving_average_t;

typedef struct {
    int window_size;
    float *sensor_buffer[NUM_SENSORS];
} moving_average_params_t;

void* moving_average(void* arg);
void update_buffer(float** sensor_buffer, moving_average_t* moving_averages, int sensor_index, int read_index, int window_size);

#endif // MOVING_AVERAGE_H
