#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include "sensor_simulator.h"

void die(const char *msg);
void initialize_buffers(float* sensor_buffer[], int window_size, int sensor_mask);

#endif // UTILS_H
