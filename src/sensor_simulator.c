#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "../include/sensor_simulator.h"

#define NUM_SENSORS 6
#define DATA_SIZE 16

#define PIPE_PAYLOAD_SIZE sizeof(float) * NUM_SENSORS

// buffered sensor data, initalized to 0 for disabled sensors
float sensor_buffer [NUM_SENSORS] = { 0 };

float generate_sensor_data() {
    return ((float)rand() / (float)(RAND_MAX)) * 100.0f;
}

void read_sensors(uint8_t sensors_enabled){
    float data;

    for (int i = 0; i < NUM_SENSORS; i++) {
        // check if current sensor is enabled
        if (sensors_enabled & (1<<i)) {
            data = generate_sensor_data();
            sensor_buffer[i] = data;
        }
    }
}

void simulate_sensor_data(uint8_t sensors_enabled, int sampling_rate_ms, int p) {
    srand(time(NULL));

    while (1) {
        read_sensors(sensors_enabled);

        if (write(p, sensor_buffer, PIPE_PAYLOAD_SIZE) == -1) {
            fprintf(stderr, "Error writing to pipe at line %d: %s\n", __LINE__, strerror(errno));
            exit(EXIT_FAILURE);
        }

        // sleep to maintain sampling rate
        usleep(1000*sampling_rate_ms);
    }

    // Close pipe
    if (close(p) == -1) {
        // Print error with line number
        fprintf(stderr, "Error closing pipe at line %d: %s\n", __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

