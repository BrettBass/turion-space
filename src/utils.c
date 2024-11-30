#include "../include/utils.h"

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void initialize_buffers(float* sensor_buffer[], int window_size, int sensor_mask) {
    for (int i = 0; i < NUM_SENSORS; i++) {
        if ( !(sensor_mask & (1 << i)) ) continue;
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
