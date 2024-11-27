#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include "shared_memory.h"
#include "output.h"

void* moving_average(void* arg);
void update_buffers(float** sensor_buffer, float* averages, int read_index, int window_size);

extern shared_memory_t shared_memory;

typedef struct {
    int runtime_sec;
    int window_size;
    time_t start_time;
    float **sensor_buffer;
} moving_average_params_t;

#endif // MOVING_AVERAGE_H
