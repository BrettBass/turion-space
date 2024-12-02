#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include "sensor_simulator.h"
#include "moving_average.h"

void die(const char *msg);
void cleanup(int mask, float** alloc_buffers);
void initialize_buffers(float* sensor_buffer[], int window_size, int sensor_mask);
int parse_positive_int(const char *str);
void print_help(const char* program_name);
void parse_arguments(int argc, char* argv[], sensor_params_t* sensor_params, moving_average_params_t* ma_params, int* runtime_sec);
#endif // UTILS_H
