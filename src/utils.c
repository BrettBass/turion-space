#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "../include/sensor_simulator.h"

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void initialize_buffers(float* sensor_buffer[], int window_size) {
    for (int i = 0; i < NUM_SENSORS; i++) {
        sensor_buffer[i] = malloc(window_size * sizeof(float));
        if (!sensor_buffer[i]) {
            // Free already allocated buffers before exiting
            for (int j = 0; j < i; j++) {
                free(sensor_buffer[j]);
            }
            die("Error: Failed to allocate memory for sensor buffer\n");
        }
    }
}
